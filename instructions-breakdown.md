## Operations with Immediate Operand
- ADDI
- ANDI 
- ORI
- XORI
- SLTI
- SLTIU
- SLLI
- SRLI
- SRAI

| STAGE         | OPERATIONS                               |
|-------------- | ---------------------------------------- |
| Fetch         | inst:opcode <- inst[6:0] = OP-IMM        |
|               | inst:func   <- inst[14:12] = *See list*  |
|               | valC[11:0]  <- inst[31:20]               |
|               | rs1         <- inst[19:15]               |
|               | rs2         <- *X*                       |
|               | rd          <- inst[11:7]                |
| Decode        | valA        <- R[rs1]                    |
|               | valB        <- *X*                       |
| Execute       | valE        <- ALU(valA,valC,inst:func)  |
| Memory        |                                          |
| Write Back    | R[rd]       <- valE                      |
| PC Update     | PC          <- PC + 4                    |

## Operations for Loading Upper Immediate
- LUI
- AUIPC

| STAGE         | OPERATIONS (LUI)                         | OPERATIONS (AUIPC)                       |
|-------------- | ---------------------------------------- | ---------------------------------------- |
| Fetch         | inst:opcode <- inst[6:0] = LUI           | inst:opcode <- inst[6:0] = AUIPC         |
|               | inst:func   <- *X*                       | inst:func   <- *X*                       |
|               | valC[31:12] <- inst[31:12]               | valC[31:12] <- inst[31:12]               |
|               | rs1         <- *X*                       | rs1         <- *X*                       |
|               | rs2         <- *X*                       | rs2         <- *X*                       |
|               | rd          <- inst[11:7]                | rd          <- inst[11:7]                |
| Decode        | valA        <- *X*                       | valA        <- *X*                       |
|               | valB        <- *X*                       | valB        <- *X*                       |
| Execute       | valE        <- ALU(0,valC,ADD)           | valE        <- ALU(PC,valC,ADD)          |
| Memory        |                                          |                                          |
| Write Back    | R[rd]       <- valE                      | R[rd]       <- valE                      |
| PC Update     | PC          <- PC + 4                    | PC          <- PC + 4                    |

## Operations for Register-Register Operand 
- ADD
- AND 
- OR
- XOR
- SLT
- SLTU
- SLL
- SRL
- SRA

| STAGE         | OPERATIONS                               |
|-------------- | ---------------------------------------- |
| Fetch         | inst:opcode <- inst[6:0] = OP            |
|               | inst:func   <- {inst[31:25,              | 
|               |                 inst[14:12]}             | 
|               |                 = *See list*             |
|               | valC[11:0]  <- *X*                       |
|               | rs1         <- inst[19:15]               |
|               | rs2         <- inst[24:20]               |
|               | rd          <- inst[11:7]                |
| Decode        | valA        <- R[rs1]                    |
|               | valB        <- R[rs2]                    |
| Execute       | valE        <- ALU(valA,valB,inst:func)  |
| Memory        |                                          |
| Write Back    | R[rd]       <- valE                      |
| PC Update     | PC          <- PC + 4                    |

## Unconditional Jumps
- JAL
- JALR

| STAGE         | OPERATIONS (JAL)                         | OPERATIONS (JALR)                        |
|-------------- | ---------------------------------------- | ---------------------------------------- |
| Fetch         | inst:opcode <- inst[6:0] = JAL           | inst:opcode <- inst[6:0] = JALR          |
|               | inst:func   <- *X*                       | inst:func   <- *X*                       | 
|               | valC[20:1]  <- {inst[31], inst[19:12],   | valC[11:0]  <- inst[31:20]               |
|               |                 inst[20], inst[30:21]}   |                                          |
|               | rs1         <- *X*                       | rs1         <- inst[19:15]               |
|               | rs2         <- *X*                       | rs2         <- *X*                       |
|               | rd          <- inst[11:7]                | rd          <- inst[11:7]                |
| Decode        | valA        <- *X*                       | valA        <- R[rs1]                    |
|               | valB        <- *X*                       | valB        <- *X*                       |
| Execute       | valE        <- ALU(PC,valC,ADD)          | valE        <- ALU(valA,valC,ADD)        |
| Memory        |                                          |                                          |
| Write Back    | R[rd]       <- valE                      | R[rd]       <- valE                      |
| PC Update     | PC          <- valE + 4                  | PC          <- valE + 4                  |

# Conditional Branch
- BEQ
- BNE
- BLT 
- BLTU
- BGE
- BGEU

| STAGE         | OPERATIONS                                 |
|-------------- | ------------------------------------------ |
| Fetch         | inst:opcode <- inst[6:0] = BRANCH          |
|               | inst:func   <- inst[14:12] = *See list*    |
|               | valC[12:1]  <- {inst[31],inst[7],          |
|               |                 inst[30:25], inst[11:8]}   |
|               | rs1         <- inst[19:15]                 |
|               | rs2         <- inst[24:20]                 |
|               | rd          <- *X*                         |
| Decode        | valA        <- R[rs1]                      |
|               | valB        <- R[rs2]                      |
| Execute       | valE        <- ALU(PC,valC,ADD)            |
|               | cond        <- comp(valA, valB, func)      |
| Memory        |                                            |
| Write Back    | R[rd]       <- *X*                         |
| PC Update     | PC          <- cond? valE + 4 : PC + 4     |

# Memory Operations
- LOAD
- STORE

| STAGE         | OPERATIONS                               | OPERATIONS                                 |
|-------------- | ---------------------------------------- | ------------------------------------------ |
| Fetch         | inst:opcode <- inst[6:0] = LOAD          | inst:opcode <- inst[6:0] = STORE           |
|               | inst:func   <- *X*                       | inst:func   <- *X*                         |
|               | valC[11:0]  <- inst[31:20]               | valC[11:0]  <- {inst[31:25], inst[4:0]}    |
|               | rs1         <- inst[19:15]               | rs1         <- inst[19:15]                 |
|               | rs2         <- *X*                       | rs2         <- inst[24:20]                 |
|               | rd          <- inst[11:7]                | rd          <- *X*                         |
| Decode        | valA        <- R[rs1]                    | valA        <- R[rs1]                      |
|               | valB        <- *X*                       | valB        <- R[rs2]                      |
| Execute       | valE        <- ALU(valA,valC,ADD)        | valE        <- ALU(valA,valC,ADD)          |
| Memory        | valM        <- M[valE]                   | M[valE]     <- valB                        |
| Write Back    | R[rd]       <- valM                      | R[rd]       <- *X*                         |
| PC Update     | PC          <- PC + 4                    | PC          <- valE : PC + 4               |
