package main

import (
	"fmt"

	"time"

	"os"
	"os/exec"
	"os/signal"

	"strings"
	"strconv"

	"io/ioutil"
	"encoding/json"
)

var (
	CONFIG_PATH = os.Getenv("HOME") + "/.config/i3sb"

	blocks []*Block
	dirty bool = true;
)

type Config struct {
	Blocks []Block	`json:"blocks"`
	Sepp string			`json:"sepperator"`
}

type Block struct {
	Cmd string	`json:"cmd"`	// Command to run 
	Out string	`json:"out"`	// Last output from cmd exec

	Sig int		`json:"sig"`	// GNU Signal to reload this block
	
	Start int	`json:"start"`	// How offen it updates
	Time int		`json:"time"`	// Time to update
}

func (b *Block) tick() {
	if b.Time > 0 {	
		b.Time--
	}	
	if b.Time == 0 {
		b.update()
		b.Time = b.Start
	}
}

func (b *Block) update() {
	out, err := exec.Command(b.Cmd).Output()
   if err != nil {
		b.Out = "ERROR " + b.Cmd;
	} else {
		b.Out = string(out);
	}
	dirty = true;	
}



func handleSignal(sig os.Signal) {
//	syscall.Signal(45)
	switch sig {
	case os.Interrupt: 
		os.Exit(1)
	default:
		if strings.HasPrefix(sig.String(), "signal ") {
			v := strings.Split(sig.String(), " ")
			i, _ := strconv.Atoi(v[1])

			if i > 34 && i <= 64 {
				for _, b := range blocks {
					if b.Sig == i {
						b.update()
					}
				}
			}
		}
	}
}

func printTop() {
	fmt.Println(`{ "version": 1 }[[],`)
}

func printBar() { 
	fmt.Print(`[{ "full_text": "`)
	fmt.Print(" | ")		
	for _, b := range blocks {
		fmt.Print(b.Out)
		fmt.Print(" | ")		
	}
	fmt.Println(`", "separator": false, "separator_block_width": 0}],`)	
}

func tryLoadConfig() {
	var err error
	
	if _, err = os.Stat(CONFIG_PATH+"/config.json"); err == nil {
		c := &Config{}
		var file []byte	

		file, err = ioutil.ReadFile(CONFIG_PATH+"/config.json")

		err = json.Unmarshal([]byte(file), c)

		if err != nil {
//			fmt.Println("Error", err)
		}

		for i, b := range c.Blocks {
			b.Time = 1
			b.Out = ""
			blocks = append(blocks, &c.Blocks[i])
		}

		//blocks = c.Blocks
	} else {
		blocks = []*Block{
			&Block{Cmd: "i3sb-date", Sig: 34, Start: 60, Time: 1},
		}
	}	
}



func main() {
//	fmt.Println(os.Getpid())
	tryLoadConfig()

	dirty = true	

	/* Setup signals */
	signals := make(chan os.Signal)
	signal.Notify(signals)
	
	go func() {
		for {
			sig := <-signals;	
			handleSignal(sig)
		}
	}()

		printTop()
	for {
		// Go throw all blocks and check if update is needed	
		for _, b := range blocks {
			b.tick()
		}
		// If block was updated then print whole bar	
		if dirty {
			printBar()
			dirty = false
		}

		time.Sleep(1 * time.Second)
	}	
}