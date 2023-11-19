All all:
	for Example in */; \
	do \
		cd $${Example}; \
		for Build in \
			"Target=LinuxPC MultiSpacc_Target=SDL12" \
			"Target=LinuxPC MultiSpacc_Target=SDL20" \
			"Target=WindowsPC MultiSpacc_Target=SDL12" \
			"Target=WindowsPC MultiSpacc_Target=SDL20" \
			"Target=Windows9x" \
			"Target=Web" \
			"Target=Switch" \
			"Target=NDS" \
			"Target=NES" \
		; do \
			make clean; \
			make -j$$(nproc --all) $${Build}; \
		done; \
	cd ..; \
	done

Clean clean Clear clear:
	for Example in */; \
	do \
		cd $${Example}; \
		make clean; \
	cd ..; \
	done

.PHONY: All all Clean clean Clear clear