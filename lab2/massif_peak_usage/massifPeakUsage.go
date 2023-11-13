package main

import (
	"fmt"
	"os"

	"github.com/joshkunz/massif"
	"github.com/martinlindhe/unit"
)

func massifTotal(s massif.Snapshot) unit.Datasize {
	return s.MemoryHeap + s.MemoryHeapExtra + s.MemoryStack
}

func main() {
	f, err := os.Open(os.Args[1])
	if err != nil {
		panic("Failed to open massif out file")
	}
	defer f.Close()

	parsed, err := massif.Parse(f)
	if err != nil {
		panic("Failed to parse massif out file")
	}

	var max *massif.Snapshot
	for _, snap := range parsed.Snapshots {
		if max == nil || massifTotal(snap) > massifTotal(*max) {
			max = &snap
		}
	}

	fmt.Printf("Max memory usage: %f KiB (%f KiB heap + %f KiB stack)\n",
		massifTotal(*max).Kibibytes(), max.MemoryHeap.Kibibytes()+max.MemoryHeapExtra.Kibibytes(),
		max.MemoryStack.Kibibytes())
}
