package main

import (
	"bytes"
	"encoding/hex"
	"fmt"
	"io"
	"io/ioutil"
	"os"

	"tpm2.srk/cmd/tpm2/util"

	"github.com/google/go-tpm-tools/tpm2tools"
	"github.com/google/go-tpm/tpm2"
	"github.com/google/go-tpm/tpmutil"
)

const TpmSrkHandle = 0x81000001

var (
	pcrSelection       = tpm2.PCRSelection{Hash: tpm2.AlgSHA256, PCRs: []int{}}
	defaultSrkTemplate = tpm2.Public{
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
	defaultKeyParams = tpm2.Public{
		Type:       tpm2.AlgRSA,
		NameAlg:    tpm2.AlgSHA256,
		Attributes: tpm2.FlagSign | tpm2.FlagDecrypt | tpm2.FlagFixedTPM | tpm2.FlagFixedParent | tpm2.FlagSensitiveDataOrigin | tpm2.FlagUserWithAuth | tpm2.FlagNoDA,
		AuthPolicy: []byte{},
		RSAParameters: &tpm2.RSAParams{
			Symmetric: &tpm2.SymScheme{
				Alg:  tpm2.AlgNull,
				Mode: tpm2.AlgUnknown,
			},
			Sign: &tpm2.SigScheme{
				Alg:  tpm2.AlgNull,
				Hash: tpm2.AlgUnknown,
			},
			KeyBits: 2048,
		},
	}
	pubKeyFile  = "key.pub"
	privKeyFile = "key.priv"
)

func LoadWithCachedSrk(rw io.ReadWriter, publicBlob []byte, privateBlob []byte) (keyhandle tpmutil.Handle, err error) {

	srkHandle := tpmutil.Handle(TpmSrkHandle)
	key, err := tpm2tools.NewCachedKey(rw, tpm2.HandleOwner, defaultSrkTemplate, srkHandle)
	if err != nil {
		return 0, err
	}

	keyhandle, _, err = tpm2.Load(rw, key.Handle(), "", publicBlob, privateBlob)
	if err != nil {
		// srk maybe not compatible, e.g. due to different unique value which is not handled by NewCachedKey
		// delete cached srk and retry
		if err = tpm2.EvictControl(rw, "", tpm2.HandleOwner, srkHandle, srkHandle); err != nil {
			return 0, err
		}
		key, err := tpm2tools.NewCachedKey(rw, tpm2.HandleOwner, defaultSrkTemplate, srkHandle)
		if err != nil {
			return 0, err
		}
		keyhandle, _, err = tpm2.Load(rw, key.Handle(), "", publicBlob, privateBlob)
		if err != nil {
			return 0, err
		}
	}

	return keyhandle, nil

}

func createKey() error {

	rwc, err := util.OpenTPMStreamSocket(os.Getenv("TPM2_SOCKET"))
	if err != nil {
		return err
	}
	defer rwc.Close()
	handle := tpmutil.Handle(TpmSrkHandle)

	// for simplification, just regenerate SRK on key generation to make sure, it is the right SRK
	err = tpm2.EvictControl(rwc, "", tpm2.HandleOwner, handle, handle)
	// ignore error if SRK is not present in NVRAM
	if err != nil {
		switch err.(type) {
		case tpm2.HandleError:
			if err.(tpm2.HandleError).Code != 11 && err.(tpm2.HandleError).Handle != tpm2.RC2 {
				return err
			}
		default:
			return err
		}
	}
	key, err := tpm2tools.NewCachedKey(rwc, tpm2.HandleOwner, defaultSrkTemplate, handle)
	if err != nil {
		return err
	}
	defer func() {
		tpm2.FlushContext(rwc, key.Handle())
	}()

	privKey, pubKey, _, _, _, err := tpm2.CreateKey(rwc, key.Handle(), pcrSelection, "", "", defaultKeyParams)
	if err != nil {
		return err
	}

	buf := &bytes.Buffer{}
	tmpKey := tpmutil.U16Bytes(pubKey)
	err = tmpKey.TPMMarshal(buf)
	if err != nil {
		return err
	}

	err = util.StoreFile(bytes.NewReader(buf.Bytes()), pubKeyFile)
	if err != nil {
		return err
	}

	buf.Reset()
	tmpKey = tpmutil.U16Bytes(privKey)
	err = tmpKey.TPMMarshal(buf)
	if err != nil {
		return err
	}

	return util.StoreFile(bytes.NewReader(buf.Bytes()), privKeyFile)

}

func loadKey() error {
	file, err := os.OpenFile(privKeyFile, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer file.Close()
	byteValueTpmPriv, err := ioutil.ReadAll(file)
	if err != nil {
		return err
	}

	pubFile, err := os.OpenFile(pubKeyFile, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer file.Close()

	byteValueTpmPub, err := ioutil.ReadAll(pubFile)
	if err != nil {
		return err
	}

	tpmPubBlob := tpmutil.U16Bytes(byteValueTpmPub)
	buf := bytes.NewBuffer(tpmPubBlob)
	err = tpmPubBlob.TPMUnmarshal(buf)
	if err != nil {
		return err
	}
	tpmPrivBlob := tpmutil.U16Bytes(byteValueTpmPriv)
	buf = bytes.NewBuffer(tpmPrivBlob)
	err = tpmPrivBlob.TPMUnmarshal(buf)
	if err != nil {
		return err
	}

	// check if private and public blobs really correspond
	rwc, err := util.OpenTPMStreamSocket(os.Getenv("TPM2_SOCKET"))
	if err != nil {
		return err
	}
	defer rwc.Close()

	tpmKeyHandle, err := LoadWithCachedSrk(rwc, tpmPubBlob, tpmPrivBlob)
	if err != nil {
		return err
	}
	defer tpm2.FlushContext(rwc, tpmKeyHandle)

	_, name, _, err := tpm2.ReadPublic(rwc, tpmKeyHandle)
	if err != nil {
		return err
	}

	fmt.Printf("public key name: %s", hex.EncodeToString(name))

	return nil
}

func main() {
	arg := os.Args[1]

	if arg == "create" {
		createKey()
	} else if arg == "load" {
		loadKey()
	}
}
