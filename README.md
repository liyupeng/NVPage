NV-Page Project Description:

The non-volatile memory such as PCM or NV-DIMM is coming. It makes possible that persistent data can be accessed in dram way.
However, the current design of linux (page cache, no distinguish between dram and pram) will make trouble: software overhead and resouce usage.
So it is necessary to make future OS support persistent ram by using a new foundational mechanism, which is the target of Project NVPage.

In this project, we divide page to V-Page and NV-Page which can be recovered during reboot. There are two featrue we provided:
1. Kernal programmer only need declare a page as the NV-Page, and the recover work during reboot is transparent.
2. Kernal programmer only need declare a set of NV-Page increasingly when the set state is legal, the work to maintain consistency during power exception is transparent.

The code of project consist of two part: one is modified Gem5 to support pram and the other one is modified linux kernal to support NV-Page.
