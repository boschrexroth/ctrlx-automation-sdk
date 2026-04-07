# TPM2

## Introduction

The ctrlX CORE devices provide the capability to securely generate and store keys with a hardware security module, specifically a Trusted Platform Module 2.0 (TPM2).

This TPM can be used additionally to Certificate Management described in __Getting Started - Package Manifest__.

## Connecting apps to the TPM2

The app.deviceadmin provides a socket interface, which allows to use the TPM2. Behind the scenes, there is a "Access Broker/Ressource Manager" working, managing concurrent sessions accessing the TPM2.

!!! check  "__Following steps are necessary to connect to TPM2 socket:__"
    <ul>
      <li>Adapt your snapcraft.yaml</li>
        <ul>
          <li>Needed plugs</li>
            <ul>
              <li>tpm2-socket: Provides your snap with the tpm2.sock file.</li>
              <li>(Optional) system-configuration: Provides a file with environment variables for configuring your TSS, called "envvars".</li>
            </ul>
          <li>Provide your app with the plugs</li>
        </ul>
      <li>Use the TPM inside your application</li>
        <ul>
          <li>Integrate the TSS as you need</li>
          <li>Source the envvars</li>
          <li>Use the TSS as usual ...</li>
        </ul>
    </ul>

More details on the Trusted Software Stack (TSS) below.

### Example snapcraft.yaml

```yaml
plugs:
  tpm2-socket:
    interface: content
    content: tpm2-socket
    target: $SNAP_DATA/tpm2-socket
  system-configuration:
    interface: content
    content: system-configuration
    target: $SNAP_DATA/system-configuration

apps:
  example-app:
    [...]
    plugs: [tpm2-socket, system-configuration, network]
```

See also the sample in [samples-snap/tpm2-webserver](./samples-snap/tpm2-webserver/README.md)

### Integration with Certificate Manager

If you provide the package-certificate slots, and use the appropriate directory structure, TPM2 usage perfectly integrates into Certificate Manager.

## Using the TPM2 inside software

After having everything in place, the usage of the TPM is straightforward. Instead of connecting to `/dev/tpmrm0`, use `$SNAP_DATA/tpm2-socket/tpm2.sock`. The file `$SNAP_DATA/system-configuration/certificate-manager/tpm2/envvars` provides you with everything needed, for example:

```bash
  export TPM2TOOLS_TCTI="cmd:nc -U $SNAP_DATA/tpm2-socket/tpm2.sock"
```

In Bash environment, you can simply source that file:

```bash
. $SNAP_DATA/system-configuration/certificate-manager/tpm2/envvars
```

### Storage Root Key (SRK)

The SRKs of the TPM is permanently stored. 

- The RSA SRK can be found at __0x81000001__ provided by TPM2_SRK_PARENT environment variable.
- The ECC SRK can be found at __0x81000002__ provided by TPM2_SRK_ECC_PARENT environment variable.

!!! note
    Although the SRKs are stored in NVRAM of TPM, it cannot be guaranteed to be always present. It must be tolerable by your application, that the SRK was replaced by another key, or even is totally absent. Same applies if your application stores
    keys in TPM2 NVRAM.

Following two examples or snippets, how to generate the SRK with the correct Primary-Key-Template.

```bash
echo "0000000: 0001" | xxd -r - unique.dat
dd if=/dev/zero bs=1 count=256 >> unique.dat
tpm2_createprimary \
    --hierarchy=owner \
    --key-algorithm=rsa2048:aes128cfb \
    --hash-algorithm=sha256 \
    --key-context=prim.ctx \
    -a 'restricted|decrypt|fixedtpm|fixedparent|sensitivedataorigin|userwithauth|noda' \
    --unique-data=unique.dat
```

In Golang, use following template:

```golang
import "github.com/google/go-tpm/tpm2"

var DefaultSrkTemplate = tpm2.Public{
		Type:       tpm2.AlgRSA,
		NameAlg:    tpm2.AlgSHA256,
		Attributes: tpm2.FlagFixedTPM | tpm2.FlagFixedParent | tpm2.FlagSensitiveDataOrigin | tpm2.FlagUserWithAuth | tpm2.FlagNoDA | tpm2.FlagRestricted | tpm2.FlagDecrypt,
		AuthPolicy: nil,
		RSAParameters: &tpm2.RSAParams{
			Symmetric: &tpm2.SymScheme{
				Alg:     tpm2.AlgAES,
				KeyBits: 128,
				Mode:    tpm2.AlgCFB,
			},
			Sign:        nil,
			KeyBits:     2048,
			ExponentRaw: 0,
			ModulusRaw:  make([]byte, 256),
		},
	}
```

For further details for golang, please see the [Golang example](./samples-go/tpm2.srk/README.md).

### Trusted Software Stack

You will need some of the following software of the TSS inside your snap, which are available as stage-packages:

- tpm2-tss [https://github.com/tpm2-software/tpm2-tss](https://github.com/tpm2-software/tpm2-tss)
- tpm2-tools [https://github.com/tpm2-software/tpm2-tools](https://github.com/tpm2-software/tpm2-tools)
- tpm2-openssl [https://github.com/tpm2-software/tpm2-openssl](https://github.com/tpm2-software/tpm2-openssl)

More information and more tools:

- [https://tpm2-software.github.io/](https://tpm2-software.github.io/)
- [https://github.com/tpm2-software](https://github.com/tpm2-software)

#### Usage with OpenSSL

For usage with OpenSSL 3, use the tpm2-openssl provider. Therefore, it is necessary to set some environment variables. Given the usage of the the `envvars` file, you can set your environment as follows:

```bash
export OPENSSL_MODULES=$SNAP/usr/lib/x86_64-linux-gnu/ossl-modules
export TPM2OPENSSL_TCTI="$TPM2TOOLS_TCTI" # will be provided by `envvars` in future releases
```

Then use OpenSSL option `-provider tpm2` to generate and handle TPM2 keys in TPM2TSS PEM format. Please see [tpm2-openssl](https://github.com/tpm2-software/tpm2-openssl) for more information about OpenSSL3 TPM2 usage.

!!! note
    TPM2 keys have different formats: tpm2-tools and TPM2TSS PEM format. TPM2TSS PEM format is mandatory for OpenSSL 3.
    While these can be converted to one another, the Device Admin can currently handle and view only native tpm2-tools keys.

### Other languages

Useful projects:

Key format conversion:

- [https://github.com/salrashid123/tpm2genkey](https://github.com/salrashid123/tpm2genkey)

Golang

- [https://github.com/google/go-tpm](https://github.com/google/go-tpm)
- [https://github.com/google/go-tpm-tools](https://github.com/google/go-tpm-tools)
- [https://github.com/salrashid123/signer](https://github.com/salrashid123/signer)

!!! attention
    If using go-tpm, you need to implement your own functions for opening, reading and writing the TPM socket, as the integrated functions close and reopen the socket with every read/write sequence. See: [https://github.com/google/go-tpm/blob/master/tpmutil/run_other.go](https://github.com/google/go-tpm/blob/master/tpmutil/run_other.go)

Python

- [https://github.com/tpm2-software/tpm2-pytss](https://github.com/tpm2-software/tpm2-pytss)
