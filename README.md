# CS570
Operating systems

## a01
Tokenizer

Compile `make`
Run `./tokenizer`

## a02
Shell

Compile `make`
Run `./mush`

## a03_threads
POSIX threads

Compile `make`
Run `./thread_demo`

## a03_pagetable
Page table

Compile `make`
Run `./pagetable [trace] [levels] [-n pages] [-p dump] [-t]`
e.g. `./pagetable trace.sample.tr 4 4 8 -n 1000 -p page_dump.txt -t`

Argument | Description
--- | ---
trace | The trace file with the page addresses to be used
levels | The bit width for each level in the page address; there will be as many levels as there are numbers provided
-n pages | The number of pages to process
-p dump | The file to dump the page table
-t | Print the page to frame translations to stdout
