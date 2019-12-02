package main

import (
	"bufio"
	"os"
)

func init() {
	go getRunes()
}

var runes = make(chan rune)

func getRunes() {
	input := bufio.NewScanner(os.Stdin)

	for input.Scan() {
		line := input.Text()
		for _, r := range line {
			runes <- r
		}
		runes <- '\n'
	}

	close(runes)
}
