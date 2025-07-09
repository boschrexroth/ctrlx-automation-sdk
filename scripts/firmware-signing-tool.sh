#!/bin/bash

usage="$(basename "$0") [-h] [-k key] [-c cabinet] [-r capsule] [-o output]

where:
    -h  show this help text
    -k  private key for signing the firmware digest 
    -c  cabinet archive containing the update capsule and metainfo.xml
    -r  a raw update capsule 
    -o  directory where the ctrlx OS firmware update archive will be placed
    
This utility packages a UEFI firmware update into a ctrlx OS firmware update archive.
You can provide either a raw update capsule or a cabinet archive containing the firmware update.
A detached signature of the firmware update will be created using the provided private key and stored in the archive.
On the target device a certificate containing the corresponding public key must be present or the firmware upload will be rejected." 
    

SIGNING_KEY=""
CAB=""
RAW=""
OUTPUT=""

while getopts "hk:c:r:o:" optionName; do
case "$optionName" in
h)  echo -e "$usage"
    exit 1
    ;;
k)  SIGNING_KEY="$OPTARG";;
c)  CAB="$OPTARG";;
r)  RAW="$OPTARG";;
o)  OUTPUT="$OPTARG";;
*)  echo -e "$usage"
esac
done

if { [ -n "$CAB" ] && [ -n "$RAW" ]; } || { [ -z "$CAB" ] && [ -z "$RAW" ]; }; then
    echo "Use either -c (for processing a cabinet archive) or -r (for processing a raw update capsule)"
    exit 1
fi


if [ -z "$SIGNING_KEY" ] || [ -z "$OUTPUT" ]; then
    echo "Missing manadtory opts -k or -o"
    exit 1 
fi

INPUT="" # Path to the firmware file
firmware="" # firmware filename

efi_guid=""
header_size=""
flags=""
capsule_size="" # Little endian size in hex
capsule_size_d="" # Decimal capsule size in bytes
efi_guid_fwupd_format=""

parse_capsule_header() {
    local input_file=$1
    echo "Parsing capsule $input_file..."
    efi_guid=$(xxd -p -l 16 "$input_file")
    header_size=$(xxd -p -l 4 -s 16 "$input_file")
    flags=$(xxd -p -l 4 -s 20 "$input_file")
    capsule_size=$(xxd -p -l 4 -s 24 "$input_file")

    echo "EFI_GUID: ${efi_guid}"
    efi_guid_fwupd_format="${efi_guid:6:2}${efi_guid:4:2}${efi_guid:2:2}${efi_guid:0:2}-${efi_guid:10:2}${efi_guid:8:2}-${efi_guid:14:2}${efi_guid:12:2}-${efi_guid:16:4}-${efi_guid:20:12}"
    echo "EFI_GUID in fwupd format: ${efi_guid_fwupd_format}"
    header_size_be="${header_size:6:2}${header_size:4:2}${header_size:2:2}${header_size:0:2}"
    echo "HeaderSize: 0x${header_size_be} | $(printf '%i' "0x$header_size_be") byte"
    echo "Flags: ${flags}"
    capsule_size_be="${capsule_size:6:2}${capsule_size:4:2}${capsule_size:2:2}${capsule_size:0:2}"
    printf -v capsule_size_d "%d" "0x$capsule_size_be"
    capsule_size_kb=$((capsule_size_d/1024))
    echo "CapsuleImageSize: 0x${capsule_size_be} | ${capsule_size_kb} kB"
}


# PROCESS CABINET ARCHIVE
if [ -n "$CAB" ]; then

  # requires cabextract

  CABINETDIR=$OUTPUT/cabinet && mkdir -p "$CABINETDIR"
  cabextract -d "$CABINETDIR" "$CAB"

  # Assert that exactly one update capsule (*.bin) and one *.metainfo.xml file are persent in the cabinet archive
  bin_count=$(find "$CABINETDIR" -name "*.bin" | wc -l)
  if [ "$bin_count" -ne 1 ]; then
      echo "Error: Cabinet archive is not as expected. Not exaclty one *.bin file in $CABINETDIR: $bin_count"
      exit 1
  fi
  metainfo_count=$(find "$CABINETDIR" -name "*.metainfo.xml" | wc -l)
  if [ "$metainfo_count" -ne 1 ]; then
      echo "Error: Cabinet archive is not as expected. Not exaclty one *.metainfo.xml file in $CABINETDIR: $metainfo_count"
      exit 1
  fi

  # Find update capsule and metainfo.xml
  INPUT=$(find "$CABINETDIR" -name "*.bin" -print -quit)
  metainfo_xml=$(find "$CABINETDIR" -name "*.metainfo.xml" -print -quit)

  parse_capsule_header "$INPUT"

  # Check if it is a FMP capsule
  if [ "$efi_guid_fwupd_format" = "6dcbd5ed-e82d-4c44-bda1-7194199ad92a" ]; then
      echo "FMP support for the capsule detected."
  else
      # Check if the EFI_CAPSULE_HEADER of the capsule is ok and contains the expected EFI_GUID
      fw=$(cat "$metainfo_xml" | grep '<firmware type="flashed">.*</firmware>' )
      fw_count=$(echo "$fw" | wc -l)
      if [ "$fw_count" -ne 1 ]; then
          echo "Update does not target exactly one firmware! Can not validate capsule header..."
      else
          expected_guid=$(cat $metainfo_xml | grep '<firmware type="flashed">.*</firmware>' | awk -F '[><]' '{print $3}')
          if [ "$efi_guid_fwupd_format" != "$expected_guid" ]; then
              echo "!!! WARNING !!!"
              echo "!!! EFI_GUID does not match the expected GUID in metainfo.xml!"
              echo "!!! (Expected) $expected_guid != (Got) $efi_guid_fwupd_format"
              echo "!!! This does not seem to be a UEFI update capsule or the EFI_CAPSULE_HEADER header is not as expected"
              echo "!!! We recommend you to approach your UEFI firmware vendor and ask them to supply you with a UEFI update capsule"
              exit 1
          fi
      fi
  fi
fi

# RAW UPDATE CAPSULE
if [ -n "$RAW" ]; then
  INPUT=$RAW

  parse_capsule_header "$INPUT"
fi

# Sanity check capsule size on disk against definition in EFI_CAPSULE_HEADER
actual_size=$(stat "$INPUT" | grep Size: | awk '{print $2}')
if [ "$capsule_size_d" != "$actual_size" ]; then
    echo "!!! WARNING !!!"
    echo "!!! capsule_size in EFI_CAPSULE_HEADER $capsule_size_d != $actual_size actual size of capsule on disk"
    echo "!!! Are you sure this is actually a compliant UEFI update capsule?"
    echo "!!! We recommend you to approach your UEFI firmware vendor and ask them to supply you with a UEFI update capsule"
    exit 1
fi

firmware=$(basename "$INPUT")

# CREATE CTRLX OS FIRMWARE UPDATE ARCHIVE
echo ">> CREATE CTRLX OS FIRMWARE UPDATE ARCHIVE"

SIGNING_DIR=$OUTPUT/temp && mkdir -p "$SIGNING_DIR"

# generate digest
firmware_digest=$(sha384sum "$INPUT" | awk '{ print $1 }')

echo ">> Singing firmware file: $INPUT"
echo ">>> digest:      $firmware_digest"
echo ">>> destination: $OUTPUT"

# create signature file
export SIGNING_DIR firmware_digest app_name publisher organization timestamp=$(date -u +"%Y-%m-%dT%H:%M:%S.%6NZ")

{
    echo "version:                1.0"
    echo "type:                   firmware"
    echo "format:                 uefi-capsule"
    echo "file:                   ${firmware}"
    echo "timestamp:              ${timestamp}"
    echo "digest:                 ${firmware_digest}"

} >> "$SIGNING_DIR"/firmware.signature

# generate signature of signature file
SIG=$(openssl dgst -sha384 -sign "$SIGNING_KEY" "$SIGNING_DIR"/firmware.signature| openssl base64)
echo -e "\n$SIG" >> "$SIGNING_DIR"/firmware.signature

# repack
cp "$INPUT" "$SIGNING_DIR"/"${firmware}".bin
tar --append --file "$OUTPUT"/"$firmware".fwupdate --directory="$SIGNING_DIR" "$firmware".bin firmware.signature
rm -r "$SIGNING_DIR"

if [ -n "$CABINETDIR" ]; then
  rm -r "$CABINETDIR"
fi


