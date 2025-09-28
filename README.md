# Riichi in Command Line

Riichi in Command Line (RiCL) is a pet project to write a networked riichi autotable, entirely in C, that runs in the command line. The goal is for RiCL to be playable on a broad range of systems.

## Usage

Build the project using make, then run the generated executable
```
make
./target/ricl
```

To reset the build directory, run
```
make clean
```

## Development Goals

### Phase One

- Auto shuffling and dealing ✓
- Local hot-seat play
- Call validation ✓
- End hand validation
- Riichi
- No scoring

### Phase Two

- Online networked play
- Client-server architecture

### Beyond/Between

- Auto-scoring?
- UI improvement

## Current Progress

- Shuffling and dealing - Done
- Local play - gameplay functions in progress
  - Draw and discard - Done
  - Pon - Done (except for turn interruption)
  - Chi - Done
  - Kan - untested
  - Riichi - not started
- Call validation - Mostly done
- UI - Basic UI mostly done
- Networked play - not started
