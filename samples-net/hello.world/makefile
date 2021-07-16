.PHONY: artifact
artifact: clean snap.amd64 snap.arm64
	
.PHONY: snap.%
snap.%:	
	cp -r publish/$* generated/
	snapcraft --target-arch=$*
	snapcraft clean
	rm -r generated/

.PHONY: clean
clean:
	snapcraft clean
