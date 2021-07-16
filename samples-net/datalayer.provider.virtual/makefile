.PHONY: artifact
artifact: clean snap.amd64 snap.arm64
	
.PHONY: snap.%
snap.%:	
	rm -f *.snap
	cp -r publish/$* generated/
	snapcraft clean
	snapcraft --target-arch=$*
	rm -r generated/

.PHONY: clean
clean:
	snapcraft clean
