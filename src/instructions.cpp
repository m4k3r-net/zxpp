#include "instructions.h"

std::unordered_map<opcode, Instruction> z80InstructionSet()
{
    std::unordered_map<opcode, Instruction> instructions;

    // NOP
    opcode oc = {0,0,0x0000};
    Instruction i = { 4, 4, 0, INST{ ; } };
    instructions[oc] = i;

    // LD BC,nn
    oc = {0,0,0x0001};
    i = { 10, 10, 2, INST{ z->getRegisters()->BC.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (BC),A
    oc = {0,0,0x0002};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->BC.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC BC
    oc = {0,0,0x0003};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = add<uint16_t>(r->BC.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC B
    oc = {0,0,0x0004};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.high == 0x7F) ? true : false;
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC B
    oc = {0,0,0x0005};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.high == 0x80) ? true : false;
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD B,n
    oc = {0,0,0x0006};
    i = { 7, 7, 1, INST{ z->getRegisters()->BC.bytes.high = d[0]; } };
    instructions[oc] = i;

    // RLCA
    oc = {0,0,0x0007};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = (bool)((r->AF.bytes.high >> 7) & 0x01);
            r->AF.bytes.high = rol<uint8_t>(r->AF.bytes.high);
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // EX AF, AF'
    oc = {0,0,0x0008};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->AFx.word;
            r->AFx.word = r->AF.word;
            r->AF.word = tmp;
        }
    };
    instructions[oc] = i;

    // ADD HL,BC
    oc = {0,0,0x0009};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add(r->HL.word, r->BC.word, r, ADD16);
            setUndocumentedFlags(r->HL.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(BC)
    oc = {0,0,0x000A};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->BC.word];
        }
    };
    instructions[oc] = i;

    // DEC BC
    oc = {0,0,0x000B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = add<uint16_t>(r->BC.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC C
    oc = {0,0,0x000C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.low == 0x7F) ? true : false;
            r->BC.bytes.low = add<uint8_t>(r->BC.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC C
    oc = {0,0,0x000D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.low == 0x80) ? true : false;
            r->BC.bytes.low = add<uint8_t>(r->BC.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD C,n
    oc = {0,0,0x000E};
    i = { 7, 7, 1, INST{ z->getRegisters()->BC.bytes.low = d[0]; } };
    instructions[oc] = i;

    // RRCA
    oc = {0,0,0x000F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = (bool)((r->AF.bytes.high) & 0x01);
            r->AF.bytes.high = ror<uint8_t>(r->AF.bytes.high);
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // DJNZ d
    oc = {0,0,0x0010};
    i = { 8, 13, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, -1, r, 0);
            if (r->BC.bytes.high != 0)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD DE,nn
    oc = {0,0,0x0011};
    i = { 10, 10, 2, INST{ z->getRegisters()->DE.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (DE),A
    oc = {0,0,0x0012};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->DE.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC DE
    oc = {0,0,0x0013};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = add<uint16_t>(r->DE.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC D
    oc = {0,0,0x0014};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.high == 0x7F) ? true : false;
            r->DE.bytes.high = add<uint8_t>(r->DE.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC D
    oc = {0,0,0x0015};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.high == 0x80) ? true : false;
            r->DE.bytes.high = add<uint8_t>(r->DE.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD D,n
    oc = {0,0,0x0016};
    i = { 7, 7, 1, INST{ z->getRegisters()->DE.bytes.high = d[0]; } };
    instructions[oc] = i;

    // RLA
    oc = {0,0,0x0017};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = rolc(r->AF.bytes.high, r->AF.bytes.low.CF);
            r->AF.bytes.low.HF = false;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // JR d
    oc = {0,0,0x0018};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->PC += d[0];
        }
    };
    instructions[oc] = i;

    // ADD HL,DE
    oc = {0,0,0x0019};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add(r->HL.word, r->DE.word, r, ADD16);
            setUndocumentedFlags(r->HL.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(DE)
    oc = {0,0,0x001A};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->DE.word];
        }
    };
    instructions[oc] = i;

    // DEC DE
    oc = {0,0,0x001B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = add<uint16_t>(r->DE.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC E
    oc = {0,0,0x001C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.low == 0x7F) ? true : false;
            r->DE.bytes.low = add<uint8_t>(r->DE.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC E
    oc = {0,0,0x001D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.low == 0x80) ? true : false;
            r->DE.bytes.low = add<uint8_t>(r->DE.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD E,n
    oc = {0,0,0x001E};
    i = { 7, 7, 1, INST{ z->getRegisters()->DE.bytes.low = d[0]; } };
    instructions[oc] = i;

    // RRA
    oc = {0,0,0x001F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = rorc(r->AF.bytes.high, r->AF.bytes.low.CF);
            r->AF.bytes.low.HF = false;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // JR NZ,d
    oc = {0,0,0x0020};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.ZF == false)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD HL,nn
    oc = {0,0,0x0021};
    i = { 10, 10, 2, INST{ z->getRegisters()->HL.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (nn),HL
    oc = {0,0,0x0022};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->HL.bytes.low;
            (*m)[nn+1] = z->getRegisters()->HL.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC HL
    oc = {0,0,0x0023};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add<uint16_t>(r->HL.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC H
    oc = {0,0,0x0024};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->HL.bytes.high == 0x7F) ? true : false;
            r->HL.bytes.high = add<uint8_t>(r->HL.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC H
    oc = {0,0,0x0025};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->HL.bytes.high == 0x80) ? true : false;
            r->HL.bytes.high = add<uint8_t>(r->HL.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD H,n
    oc = {0,0,0x0026};
    i = { 7, 7, 1, INST{ z->getRegisters()->HL.bytes.high = d[0]; } };
    instructions[oc] = i;

    // DAA
    oc = {0,0,0x0027};
    i = { 4, 4, 0, INST{ Z80Registers* r = z->getRegisters(); daa(r); } };
    instructions[oc] = i;

    // JR Z,d
    oc = {0,0,0x0028};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.ZF == true)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // ADD HL,HL
    oc = {0,0,0x0029};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add(r->HL.word, r->HL.word, r, ADD16);
            setUndocumentedFlags(r->HL.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD HL,(nn)
    oc = {0,0,0x002A};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            uint16_t in = CREATE_WORD((*m)[nn], (*m)[nn+1]);
            z->getRegisters()->HL.word = in;
        }
    };
    instructions[oc] = i;

    // DEC HL
    oc = {0,0,0x002B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add<uint16_t>(r->HL.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC L
    oc = {0,0,0x002C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->HL.bytes.low == 0x7F) ? true : false;
            r->HL.bytes.low = add<uint8_t>(r->HL.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC L
    oc = {0,0,0x002D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->HL.bytes.low == 0x80) ? true : false;
            r->HL.bytes.low = add<uint8_t>(r->HL.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD L,n
    oc = {0,0,0x002E};
    i = { 7, 7, 1, INST{ z->getRegisters()->HL.bytes.low = d[0]; } };
    instructions[oc] = i;

    // CPL
    oc = {0,0,0x002F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = ~(r->AF.bytes.high);
            r->AF.bytes.low.HF = true;
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // JR NC,d
    oc = {0,0,0x0030};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.CF == false)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD SP,nn
    oc = {0,0,0x0031};
    i = { 10, 10, 2, INST{ z->getRegisters()->SP = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (nn),A
    oc = {0,0,0x0032};
    i = { 13, 13, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC SP
    oc = {0,0,0x0033};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = add<uint16_t>(r->SP, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC (HL)
    oc = {0,0,0x0034};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t byte = (*m)[r->HL.word];
            r->AF.bytes.low.PF = (byte == 0x7F) ? true : false;
            byte = add<uint8_t>(byte, 1, r, INC8);
            (*m)[r->HL.word] = byte;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC (HL)
    oc = {0,0,0x0035};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t byte = (*m)[r->HL.word];
            r->AF.bytes.low.PF = (byte == 0x80) ? true : false;
            byte = add<uint8_t>(byte, -1, r, DEC8);
            (*m)[r->HL.word] = byte;
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD (HL),n
    oc = {0,0,0x0036};
    i = { 10, 10, 0, INST{
            (*m)[z->getRegisters()->HL.word] = d[0];
        }
    };
    instructions[oc] = i;

    // SCF
    oc = {0,0,0x0037};
    i = { 4, 4, 0, INST{
            z->getRegisters()->AF.bytes.low.CF = true;
            z->getRegisters()->AF.bytes.low.NF = false;
            z->getRegisters()->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // JR C,d
    oc = {0,0,0x0038};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.CF == true)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // ADD HL,SP
    oc = {0,0,0x0039};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add(r->HL.word, r->SP, r, ADD16);
            setUndocumentedFlags(r->HL.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(nn)
    oc = {0,0,0x003A};
    i = { 13, 13, 0, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            z->getRegisters()->AF.bytes.high = (*m)[nn];
        }
    };
    instructions[oc] = i;

    // DEC SP
    oc = {0,0,0x003B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = add<uint16_t>(r->SP, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC A
    oc = {0,0,0x003C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->AF.bytes.high == 0x7F) ? true : false;
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC A
    oc = {0,0,0x003D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->AF.bytes.high == 0x80) ? true : false;
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD A,n
    oc = {0,0,0x003E};
    i = { 7, 7, 1, INST{ z->getRegisters()->AF.bytes.high = d[0]; } };
    instructions[oc] = i;

    // CCF
    oc = {0,0,0x003F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            bool prevCarry = r->AF.bytes.low.CF;
            r->AF.bytes.low.CF = (r->AF.bytes.low.CF) ? false : true;
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = prevCarry;
        }
    };
    instructions[oc] = i;

    // LD B,B
    oc = {0,0,0x0040};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ }};
    instructions[oc] = i;

    // LD B,C
    oc = {0,0,0x0041};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD B,D
    oc = {0,0,0x0042};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD B,E
    oc = {0,0,0x0043};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD B,H
    oc = {0,0,0x0044};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD B,L
    oc = {0,0,0x0045};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->HL.bytes.low; } };
    instructions[oc] = i;

    // LD B,(HL)
    oc = {0,0,0x0046};
    i = { 7, 7, 0, INST{
            z->getRegisters()->BC.bytes.high = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD B,A
    oc = {0,0,0x0047};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD C,B
    oc = {0,0,0x0048};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD C,C
    oc = {0,0,0x0049};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD C,D
    oc = {0,0,0x004A};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD C,E
    oc = {0,0,0x004B};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD C,H
    oc = {0,0,0x004C};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD C,L
    oc = {0,0,0x004D};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->HL.bytes.low; } };
    instructions[oc] = i;

    // LD C,(HL)
    oc = {0,0,0x004E};
    i = { 7, 7, 0, INST{
            z->getRegisters()->BC.bytes.low = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD C,A
    oc = {0,0,0x004F};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD D,B
    oc = {0,0,0x0050};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD D,C
    oc = {0,0,0x0051};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD D,D
    oc = {0,0,0x0052};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD D,E
    oc = {0,0,0x0053};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD D,H
    oc = {0,0,0x0054};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD D,L
    oc = {0,0,0x0055};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->HL.bytes.low; } };
    instructions[oc] = i;

    // LD D,(HL)
    oc = {0,0,0x0056};
    i = { 7, 7, 0, INST{
            z->getRegisters()->DE.bytes.high = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD D,A
    oc = {0,0,0x0057};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD E,B
    oc = {0,0,0x0058};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD E,C
    oc = {0,0,0x0059};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD E,D
    oc = {0,0,0x005A};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD E,E
    oc = {0,0,0x005B};
    i = { 4, 4, 0, INST{; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD E,H
    oc = {0,0,0x005C};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD E,L
    oc = {0,0,0x005D};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->HL.bytes.low; } };
    instructions[oc] = i;

    // LD E,(HL)
    oc = {0,0,0x005E};
    i = { 7, 7, 0, INST{
            z->getRegisters()->DE.bytes.low = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD E,A
    oc = {0,0,0x005F};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD H,B
    oc = {0,0,0x0060};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD H,C
    oc = {0,0,0x0061};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD H,D
    oc = {0,0,0x0062};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD H,E
    oc = {0,0,0x0063};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD H,H
    oc = {0,0,0x0064};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD H,L
    oc = {0,0,0x0065};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.high = z->getRegisters()->HL.bytes.low; } };
    instructions[oc] = i;

    // LD H,(HL)
    oc = {0,0,0x0066};
    i = { 7, 7, 0, INST{
            z->getRegisters()->HL.bytes.high = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD H,A
    oc = {0,0,0x0067};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD L,B
    oc = {0,0,0x0068};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD L,C
    oc = {0,0,0x0069};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.low = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD L,D
    oc = {0,0,0x006A};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD L,E
    oc = {0,0,0x006B};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.low = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD L,H
    oc = {0,0,0x006C};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.low = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD L,L
    oc = {0,0,0x006D};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD L,(HL)
    oc = {0,0,0x006E};
    i = { 7, 7, 0, INST{
            z->getRegisters()->HL.bytes.low = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD L,A
    oc = {0,0,0x006F};
    i = { 4, 4, 0, INST{ z->getRegisters()->HL.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD (HL),B
    oc = {0,0,0x0070};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->BC.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (HL),C
    oc = {0,0,0x0071};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // LD (HL),D
    oc = {0,0,0x0072};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->DE.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (HL),E
    oc = {0,0,0x0073};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->DE.bytes.low;
        }
    };
    instructions[oc] = i;

    // LD (HL),H
    oc = {0,0,0x0074};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->HL.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (HL),L
    oc = {0,0,0x0075};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->HL.bytes.low;
        }
    };
    instructions[oc] = i;

    // HALT
    oc = {0,0,0x0076};
    i = { 4, 4, 0, INST{
            z->halt();
        }
    };
    instructions[oc] = i;

    // LD (HL),A
    oc = {0,0,0x0077};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->HL.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD A,B
    oc = {0,0,0x0078};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD A,C
    oc = {0,0,0x0079};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD A,D
    oc = {0,0,0x007A};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD A,E
    oc = {0,0,0x007B};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD A,H
    oc = {0,0,0x007C};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD A,L
    oc = {0,0,0x007D};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->HL.bytes.low; } };
    instructions[oc] = i;

    // LD A,(HL)
    oc = {0,0,0x007E};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->HL.word];
        }
    };
    instructions[oc] = i;

    // LD A,A
    oc = {0,0,0x007F};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // ADD A,B
    oc = {0,0,0x0080};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,C
    oc = {0,0,0x0081};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,D
    oc = {0,0,0x0082};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,E
    oc = {0,0,0x0083};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,H
    oc = {0,0,0x0084};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->HL.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,L
    oc = {0,0,0x0085};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->HL.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,(HL)
    oc = {0,0,0x0086};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, (*m)[z->getRegisters()->HL.word], r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,A
    oc = {0,0,0x0087};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->AF.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,B
    oc = {0,0,0x0088};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,C
    oc = {0,0,0x0089};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,D
    oc = {0,0,0x008A};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,E
    oc = {0,0,0x008B};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,H
    oc = {0,0,0x008C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->HL.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,L
    oc = {0,0,0x008D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->HL.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,(HL)
    oc = {0,0,0x008E};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, (*m)[z->getRegisters()->HL.word], r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,A
    oc = {0,0,0x008F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->AF.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // SUB B
    oc = {0,0,0x0090};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB C
    oc = {0,0,0x0091};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB D
    oc = {0,0,0x0092};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB E
    oc = {0,0,0x0093};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB H
    oc = {0,0,0x0094};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->HL.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB L
    oc = {0,0,0x0095};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->HL.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB (HL)
    oc = {0,0,0x0096};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->HL.word]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB A
    oc = {0,0,0x0097};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,B
    oc = {0,0,0x0098};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,C
    oc = {0,0,0x0099};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,D
    oc = {0,0,0x009A};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,E
    oc = {0,0,0x009B};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,H
    oc = {0,0,0x009C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->HL.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,L
    oc = {0,0,0x009D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->HL.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,(HL)
    oc = {0,0,0x009E};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->HL.word]), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,A
    oc = {0,0,0x009F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // AND B
    oc = {0,0,0x00A0};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND C
    oc = {0,0,0x00A1};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND D
    oc = {0,0,0x00A2};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND E
    oc = {0,0,0x00A3};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND H
    oc = {0,0,0x00A4};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->HL.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND L
    oc = {0,0,0x00A5};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->HL.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND (HL)
    oc = {0,0,0x00A6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, (*m)[z->getRegisters()->HL.word], r);
        }
    };
    instructions[oc] = i;

    // AND A
    oc = {0,0,0x00A7};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR B
    oc = {0,0,0x00A8};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR C
    oc = {0,0,0x00A9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR D
    oc = {0,0,0x00AA};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR E
    oc = {0,0,0x00AB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR H
    oc = {0,0,0x00AC};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->HL.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR L
    oc = {0,0,0x00AD};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->HL.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR (HL)
    oc = {0,0,0x00AE};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, (*m)[z->getRegisters()->HL.word], r);
        }
    };
    instructions[oc] = i;

    // XOR A
    oc = {0,0,0x00AF};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR B
    oc = {0,0,0x00B0};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR C
    oc = {0,0,0x00B1};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR D
    oc = {0,0,0x00B2};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR E
    oc = {0,0,0x00B3};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR H
    oc = {0,0,0x00B4};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->HL.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR L
    oc = {0,0,0x00B5};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->HL.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR (HL)
    oc = {0,0,0x00B6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, (*m)[z->getRegisters()->HL.word], r);
        }
    };
    instructions[oc] = i;

    // OR A
    oc = {0,0,0x00B7};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // CP B
    oc = {0,0,0x00B8};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP C
    oc = {0,0,0x00B9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;            
        }
    };
    instructions[oc] = i;

    // CP D
    oc = {0,0,0x00BA};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP E
    oc = {0,0,0x00BB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP H
    oc = {0,0,0x00BC};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->HL.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP L
    oc = {0,0,0x00BD};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->HL.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP (HL)
    oc = {0,0,0x00BE};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->HL.word]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP A
    oc = {0,0,0x00BF};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RET NZ
    oc = {0,0,0x00C0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::NZ);
        }
    };
    instructions[oc] = i;

    // POP BC
    oc = {0,0,0x00C1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP NZ,nn
    oc = {0,0,0x00C2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.ZF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // JP nn
    oc = {0,0,0x00C3};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            r->PC = nn;
        }
    };
    instructions[oc] = i;

    // CALL NZ,nn
    oc = {0,0,0x00C4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::NZ, nn);
        }
    };
    instructions[oc] = i;

    // PUSH BC
    oc = {0,0,0x00C5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->BC.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // ADD A,n
    oc = {0,0,0x00C6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, d[0], r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // RST 00
    oc = {0,0,0x00C7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x00;
        }
    };
    instructions[oc] = i;

    // RET Z
    oc = {0,0,0x00C8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::Z);
        }
    };
    instructions[oc] = i;

    // RET
    oc = {0,0,0x00C9};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = 0 | (*m)[r->SP];
            r->SP++;
            r->PC |= ((*m)[r->SP]) << 8;
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP Z,nn
    oc = {0,0,0x00CA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.ZF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // 0x00CB is prefix

    // CALL Z,nn
    oc = {0,0,0x00CC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::Z, nn);
        }
    };
    instructions[oc] = i;

    // CALL nn
    oc = {0,0,0x00CD};
    i = { 17, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            r->SP--;
            (*m)[r->SP] = (r->PC) >> 8;
            r->SP--;
            (*m)[r->SP] = (r->PC) & 0xFF;
            r->PC = nn;
        }
    };
    instructions[oc] = i;

    // ADC A,n
    oc = {0,0,0x00CE};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, d[0], r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // RST 08
    oc = {0,0,0x00CF};  
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x08;
        }
    };
    instructions[oc] = i;

    // RET NC
    oc = {0,0,0x00D0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::NC);
        }
    };
    instructions[oc] = i;

    // POP DE
    oc = {0,0,0x00D1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP NC,nn
    oc = {0,0,0x00D2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.CF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // OUT (n),A
    oc = {0,0,0x00D3};
    i = { 11, 11, 1, INST{
            Z80Registers* r = z->getRegisters();
            (*(z->getIoPorts()))[d[0]] = r->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // CALL NC,nn
    oc = {0,0,0x00D4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::NC, nn);
        }
    };
    instructions[oc] = i;

    // PUSH DE
    oc = {0,0,0x00D5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->DE.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->DE.bytes.low;
        }
    };
    instructions[oc] = i;

    // SUB n
    oc = {0,0,0x00D6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 10
    oc = {0,0,0x00D7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x10;
        }
    };
    instructions[oc] = i;

    // RET C
    oc = {0,0,0x00D8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::C);
        }
    };
    instructions[oc] = i;

    // EXX
    oc = {0,0,0x00D9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->BC.word;
            r->BC.word = r->BCx.word;
            r->BCx.word = tmp;
            tmp = r->DE.word;
            r->DE.word = r->DEx.word;
            r->DEx.word = tmp;
            tmp = r->HL.word;
            r->HL.word = r->HLx.word;
            r->HLx.word = tmp;
        }
    };
    instructions[oc] = i;

    // JP C,nn
    oc = {0,0,0x00DA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.CF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // IN A,(n)
    oc = {0,0,0x00DB};
    i = { 11, 11, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = (*(z->getIoPorts()))[d[0]];
        }
    };
    instructions[oc] = i;

    // CALL C,nn
    oc = {0,0,0x00DC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::C, nn);
        }
    };
    instructions[oc] = i;

    // 0xDD is prefix

    // SBC A,n
    oc = {0,0,0x00DE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 18
    oc = {0,0,0x00DF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x18;
        }
    };
    instructions[oc] = i;

    // RET PO
    oc = {0,0,0x00E0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::PO);
        }
    };
    instructions[oc] = i;

    // POP HL
    oc = {0,0,0x00E1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP PO,nn
    oc = {0,0,0x00E2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.PF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EX (SP),HL
    oc = {0,0,0x00E3};
    i = { 19, 19, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t tmp = r->HL.bytes.low;
            r->HL.bytes.low = (*m)[r->SP];
            (*m)[r->SP] = tmp;
            tmp = r->HL.bytes.high;
            r->HL.bytes.high = (*m)[(r->SP)+1];
            (*m)[(r->SP)+1] = tmp;
        }
    };
    instructions[oc] = i;

    // CALL PO,nn
    oc = {0,0,0x00E4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::PO, nn);
        }
    };
    instructions[oc] = i;

    // PUSH HL
    oc = {0,0,0x00E5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->HL.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->HL.bytes.low;
        }
    };
    instructions[oc] = i;

    // AND n
    oc = {0,0,0x00E6};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 20
    oc = {0,0,0x00E7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x20;
        }
    };
    instructions[oc] = i;

    // RET PE
    oc = {0,0,0x00E8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::PE);
        }
    };
    instructions[oc] = i;

    // JP,(HL)
    oc = {0,0,0x00E9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = (*m)[r->HL.word];
        }
    };
    instructions[oc] = i;

    // JP PE,nn
    oc = {0,0,0x00EA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.PF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EX DE,HL
    oc = {0,0,0x00EB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->DE.word;
            r->DE.word = r->HL.word;
            r->HL.word = tmp;
        }
    };
    instructions[oc] = i;

    // CALL PE,nn
    oc = {0,0,0x00EC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::PE, nn);
        }
    };
    instructions[oc] = i;

    // 0xED is prefix

    // XOR n
    oc = {0,0,0x00EE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 28
    oc = {0,0,0x00EF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x28;
        }
    };
    instructions[oc] = i;

    // RET P
    oc = {0,0,0x00F0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::P);
        }
    };
    instructions[oc] = i;

    // POP AF
    oc = {0,0,0x00F1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP P,nn
    oc = {0,0,0x00F2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.SF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // DI
    oc = {0,0,0x00F3};
    i = { 4, 4, 1, INST{
            Z80Registers* r = z->getRegisters();
            z->setIFF1(false);
            z->setIFF2(false);
        }
    };
    instructions[oc] = i;

    // CALL P,nn
    oc = {0,0,0x00F4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::P, nn);
        }
    };
    instructions[oc] = i;

    // PUSH AF
    oc = {0,0,0x00F5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->AF.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->AF.bytes.low.byte;
        }
    };
    instructions[oc] = i;

    // OR n
    oc = {0,0,0x00F6};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 30
    oc = {0,0,0x00F7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x30;
        }
    };
    instructions[oc] = i;

    // RET M
    oc = {0,0,0x00F8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::M);
        }
    };
    instructions[oc] = i;

    // LD SP,HL
    oc = {0,0,0x00F9};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = r->HL.word;
        }
    };
    instructions[oc] = i;

    // JP M,nn
    oc = {0,0,0x00FA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.SF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EI
    oc = {0,0,0x00FB};
    i = { 4, 4, 1, INST{
            Z80Registers* r = z->getRegisters();
            z->setIFF1(true);
            z->setIFF2(true);
        }
    };
    instructions[oc] = i;

    // CALL M,nn
    oc = {0,0,0x00FC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::M, nn);
        }
    };
    instructions[oc] = i;

    // 0xFD is prefix

    // CP n
    oc = {0,0,0x00FE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 38
    oc = {0,0,0x00FF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x38;
        }
    };
    instructions[oc] = i;

    // NOP
    oc = {0,0xDD,0x0000};
    i = { 4, 4, 0, INST{ ; } };
    instructions[oc] = i;

    // LD BC,nn
    oc = {0,0xDD,0x0001};
    i = { 10, 10, 2, INST{ z->getRegisters()->BC.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (BC),A
    oc = {0,0xDD,0x0002};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->BC.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC BC
    oc = {0,0xDD,0x0003};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = add<uint16_t>(r->BC.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC B
    oc = {0,0xDD,0x0004};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.high == 0x7F) ? true : false;
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC B
    oc = {0,0xDD,0x0005};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.high == 0x80) ? true : false;
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD B,n
    oc = {0,0xDD,0x0006};
    i = { 7, 7, 1, INST{ z->getRegisters()->BC.bytes.high = d[0]; } };
    instructions[oc] = i;

    // RLCA
    oc = {0,0xDD,0x0007};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = (bool)((r->AF.bytes.high >> 7) & 0x01);
            r->AF.bytes.high = rol<uint8_t>(r->AF.bytes.high);
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // EX AF, AF'
    oc = {0,0xDD,0x0008};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->AFx.word;
            r->AFx.word = r->AF.word;
            r->AF.word = tmp;
        }
    };
    instructions[oc] = i;

    // ADD IX,BC
    oc = {0,0xDD,0x0009};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = add(r->IX.word, r->BC.word, r, ADD16);
            setUndocumentedFlags(r->IX.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(BC)
    oc = {0,0xDD,0x000A};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->BC.word];
        }
    };
    instructions[oc] = i;

    // DEC BC
    oc = {0,0xDD,0x000B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = add<uint16_t>(r->BC.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC C
    oc = {0,0xDD,0x000C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.low == 0x7F) ? true : false;
            r->BC.bytes.low = add<uint8_t>(r->BC.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC C
    oc = {0,0xDD,0x000D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.low == 0x80) ? true : false;
            r->BC.bytes.low = add<uint8_t>(r->BC.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD C,n
    oc = {0,0xDD,0x000E};
    i = { 7, 7, 1, INST{ z->getRegisters()->BC.bytes.low = d[0]; } };
    instructions[oc] = i;

    // RRCA
    oc = {0,0xDD,0x000F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = (bool)((r->AF.bytes.high) & 0x01);
            r->AF.bytes.high = ror<uint8_t>(r->AF.bytes.high);
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // DJNZ d
    oc = {0,0xDD,0x0010};
    i = { 8, 13, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, -1, r, 0);
            if (r->BC.bytes.high != 0)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD DE,nn
    oc = {0,0xDD,0x0011};
    i = { 10, 10, 2, INST{ z->getRegisters()->DE.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (DE),A
    oc = {0,0xDD,0x0012};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->DE.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC DE
    oc = {0,0xDD,0x0013};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = add<uint16_t>(r->DE.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC D
    oc = {0,0xDD,0x0014};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.high == 0x7F) ? true : false;
            r->DE.bytes.high = add<uint8_t>(r->DE.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC D
    oc = {0,0xDD,0x0015};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.high == 0x80) ? true : false;
            r->DE.bytes.high = add<uint8_t>(r->DE.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD D,n
    oc = {0,0xDD,0x0016};
    i = { 7, 7, 1, INST{ z->getRegisters()->DE.bytes.high = d[0]; } };
    instructions[oc] = i;

    // RLA
    oc = {0,0xDD,0x0017};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = rolc(r->AF.bytes.high, r->AF.bytes.low.CF);
            r->AF.bytes.low.HF = false;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // JR d
    oc = {0,0xDD,0x0018};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->PC += d[0];
        }
    };
    instructions[oc] = i;

    // ADD IX,DE
    oc = {0,0xDD,0x0019};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = add(r->IX.word, r->DE.word, r, ADD16);
            setUndocumentedFlags(r->IX.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(DE)
    oc = {0,0xDD,0x001A};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->DE.word];
        }
    };
    instructions[oc] = i;

    // DEC DE
    oc = {0,0xDD,0x001B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = add<uint16_t>(r->DE.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC E
    oc = {0,0xDD,0x001C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.low == 0x7F) ? true : false;
            r->DE.bytes.low = add<uint8_t>(r->DE.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC E
    oc = {0,0xDD,0x001D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.low == 0x80) ? true : false;
            r->DE.bytes.low = add<uint8_t>(r->DE.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD E,n
    oc = {0,0xDD,0x001E};
    i = { 7, 7, 1, INST{ z->getRegisters()->DE.bytes.low = d[0]; } };
    instructions[oc] = i;

    // RRA
    oc = {0,0xDD,0x001F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = rorc(r->AF.bytes.high, r->AF.bytes.low.CF);
            r->AF.bytes.low.HF = false;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // JR NZ,d
    oc = {0,0xDD,0x0020};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.ZF == false)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD IXL,nn
    oc = {0,0xDD,0x0021};
    i = { 14, 14, 2, INST{ z->getRegisters()->IX.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (nn),IX
    oc = {0,0xDD,0x0022};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->IX.bytes.low;
            (*m)[nn+1] = z->getRegisters()->IX.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC IX
    oc = {0,0xDD,0x0023};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = add<uint16_t>(r->IX.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC IXH
    oc = {0,0xDD,0x0024};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IX.bytes.high == 0x7F) ? true : false;
            r->IX.bytes.high = add<uint8_t>(r->IX.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC IXH
    oc = {0,0xDD,0x0025};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IX.bytes.high == 0x80) ? true : false;
            r->IX.bytes.high = add<uint8_t>(r->IX.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD IXH,n
    oc = {0,0xDD,0x0026};
    i = { 9, 9, 1, INST{ z->getRegisters()->IX.bytes.high = d[0]; } };
    instructions[oc] = i;

    // DAA
    oc = {0,0xDD,0x0027};
    i = { 4, 4, 0, INST{ Z80Registers* r = z->getRegisters(); daa(r); } };
    instructions[oc] = i;

    // JR Z,d
    oc = {0,0xDD,0x0028};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.ZF == true)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // ADD IX,IX
    oc = {0,0xDD,0x0029};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = add(r->IX.word, r->IX.word, r, ADD16);
            setUndocumentedFlags(r->IX.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD IX,(nn)
    oc = {0,0xDD,0x002A};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            uint16_t in = CREATE_WORD((*m)[nn], (*m)[nn+1]);
            z->getRegisters()->IX.word = in;
        }
    };
    instructions[oc] = i;

    // DEC IX
    oc = {0,0xDD,0x002B};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = add<uint16_t>(r->IX.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC IXL
    oc = {0,0xDD,0x002C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IX.bytes.low == 0x7F) ? true : false;
            r->IX.bytes.low = add<uint8_t>(r->IX.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC IXL
    oc = {0,0xDD,0x002D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IX.bytes.low == 0x80) ? true : false;
            r->IX.bytes.low = add<uint8_t>(r->IX.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD IXL,n
    oc = {0,0xDD,0x002E};
    i = { 7, 7, 1, INST{ z->getRegisters()->IX.bytes.low = d[0]; } };
    instructions[oc] = i;

    // CPL
    oc = {0,0xDD,0x002F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = ~(r->AF.bytes.high);
            r->AF.bytes.low.HF = true;
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // JR NC,d
    oc = {0,0xDD,0x0030};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.CF == false)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD SP,nn
    oc = {0,0xDD,0x0031};
    i = { 10, 10, 2, INST{ z->getRegisters()->SP = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (nn),A
    oc = {0,0xDD,0x0032};
    i = { 13, 13, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC SP
    oc = {0,0xDD,0x0033};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = add<uint16_t>(r->SP, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC (IX+d)
    oc = {0,0xDD,0x0034};
    i = { 23, 23, 1, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t byte = (*m)[r->IX.word+((int8_t)(d[0]))];
            r->AF.bytes.low.PF = (byte == 0x7F) ? true : false;
            byte = add<uint8_t>(byte, 1, r, INC8);
            (*m)[r->IX.word+((int8_t)(d[0]))] = byte;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC (IX+d)
    oc = {0,0xDD,0x0035};
    i = { 23, 23, 1, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t byte = (*m)[r->IX.word+((int8_t)(d[0]))];
            r->AF.bytes.low.PF = (byte == 0x80) ? true : false;
            byte = add<uint8_t>(byte, -1, r, DEC8);
            (*m)[r->IX.word+((int8_t)(d[0]))] = byte;
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD (IX+d),n
    oc = {0,0xDD,0x0036};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = d[0];
        }
    };
    instructions[oc] = i;

    // SCF
    oc = {0,0xDD,0x0037};
    i = { 4, 4, 0, INST{
            z->getRegisters()->AF.bytes.low.CF = true;
            z->getRegisters()->AF.bytes.low.NF = false;
            z->getRegisters()->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // JR C,d
    oc = {0,0xDD,0x0038};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.CF == true)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // ADD IX,SP
    oc = {0,0xDD,0x0039};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = add(r->IX.word, r->SP, r, ADD16);
            setUndocumentedFlags(r->IX.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(nn)
    oc = {0,0xDD,0x003A};
    i = { 13, 13, 0, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            z->getRegisters()->AF.bytes.high = (*m)[nn];
        }
    };
    instructions[oc] = i;

    // DEC SP
    oc = {0,0xDD,0x003B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = add<uint16_t>(r->SP, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC A
    oc = {0,0xDD,0x003C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->AF.bytes.high == 0x7F) ? true : false;
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC A
    oc = {0,0xDD,0x003D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->AF.bytes.high == 0x80) ? true : false;
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD A,n
    oc = {0,0xDD,0x003E};
    i = { 7, 7, 1, INST{ z->getRegisters()->AF.bytes.high = d[0]; } };
    instructions[oc] = i;

    // CCF
    oc = {0,0xDD,0x003F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            bool prevCarry = r->AF.bytes.low.CF;
            r->AF.bytes.low.CF = (r->AF.bytes.low.CF) ? false : true;
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = prevCarry;
        }
    };
    instructions[oc] = i;

    // LD B,B
    oc = {0,0xDD,0x0040};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ }};
    instructions[oc] = i;

    // LD B,C
    oc = {0,0xDD,0x0041};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD B,D
    oc = {0,0xDD,0x0042};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD B,E
    oc = {0,0xDD,0x0043};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD B,IXH
    oc = {0,0xDD,0x0044};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->IX.bytes.high; } };
    instructions[oc] = i;

    // LD B,IXL
    oc = {0,0xDD,0x0045};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->IX.bytes.low; } };
    instructions[oc] = i;

    // LD B,(IX+d)
    oc = {0,0xDD,0x0046};
    i = { 19, 19, 1, INST{
            z->getRegisters()->BC.bytes.high = (*m)[z->getRegisters()->IX.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD B,A
    oc = {0,0xDD,0x0047};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD C,B
    oc = {0,0xDD,0x0048};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD C,C
    oc = {0,0xDD,0x0049};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD C,D
    oc = {0,0xDD,0x004A};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD C,E
    oc = {0,0xDD,0x004B};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD C,IXH
    oc = {0,0xDD,0x004C};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->IX.bytes.high; } };
    instructions[oc] = i;

    // LD C,IXL
    oc = {0,0xDD,0x004D};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->IX.bytes.low; } };
    instructions[oc] = i;

    // LD C,(IX+d)
    oc = {0,0xDD,0x004E};
    i = { 7, 7, 1, INST{
            z->getRegisters()->BC.bytes.low = (*m)[z->getRegisters()->IX.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD C,A
    oc = {0,0xDD,0x004F};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD D,B
    oc = {0,0xDD,0x0050};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD D,C
    oc = {0,0xDD,0x0051};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD D,D
    oc = {0,0xDD,0x0052};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD D,E
    oc = {0,0xDD,0x0053};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD D,IXH
    oc = {0,0xDD,0x0054};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->IX.bytes.high; } };
    instructions[oc] = i;

    // LD D,IXL
    oc = {0,0xDD,0x0055};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->IX.bytes.low; } };
    instructions[oc] = i;

    // LD D,(IX+d)
    oc = {0,0xDD,0x0056};
    i = { 19, 19, 1, INST{
            z->getRegisters()->DE.bytes.high = (*m)[z->getRegisters()->IX.word+((int8_t)(d[0]))];
        }
    };
    instructions[oc] = i;

    // LD D,A
    oc = {0,0xDD,0x0057};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD E,B
    oc = {0,0xDD,0x0058};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD E,C
    oc = {0,0xDD,0x0059};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD E,D
    oc = {0,0xDD,0x005A};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD E,E
    oc = {0,0xDD,0x005B};
    i = { 4, 4, 0, INST{; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD E,IXH
    oc = {0,0xDD,0x005C};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->IX.bytes.high; } };
    instructions[oc] = i;

    // LD E,IXL
    oc = {0,0xDD,0x005D};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->IX.bytes.low; } };
    instructions[oc] = i;

    // LD E,(IX+d)
    oc = {0,0xDD,0x005E};
    i = { 19, 19, 1, INST{
            z->getRegisters()->DE.bytes.low = (*m)[z->getRegisters()->IX.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD E,A
    oc = {0,0xDD,0x005F};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD IXH,B
    oc = {0,0xDD,0x0060};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD IXH,C
    oc = {0,0xDD,0x0061};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD IXH,D
    oc = {0,0xDD,0x0062};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD IXH,E
    oc = {0,0xDD,0x0063};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD IXH,IXH
    oc = {0,0xDD,0x0064};
    i = { 9, 9, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD IXH,IXL
    oc = {0,0xDD,0x0065};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.high = z->getRegisters()->IX.bytes.low; } };
    instructions[oc] = i;

    // LD H,(IX+d)
    oc = {0,0xDD,0x0066};
    i = { 19, 19, 1, INST{
            z->getRegisters()->HL.bytes.high = (*m)[z->getRegisters()->IX.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD IXH,A
    oc = {0,0xDD,0x0067};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD IXL,B
    oc = {0,0xDD,0x0068};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD IXL,C
    oc = {0,0xDD,0x0069};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.low = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD IXL,D
    oc = {0,0xDD,0x006A};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD IXL,E
    oc = {0,0xDD,0x006B};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.low = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD IXL,H
    oc = {0,0xDD,0x006C};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.low = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD IXL,IXL
    oc = {0,0xDD,0x006D};
    i = { 9, 9, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD L,(IX+d)
    oc = {0,0xDD,0x006E};
    i = { 19, 19, 1, INST{
            z->getRegisters()->HL.bytes.low = (*m)[z->getRegisters()->IX.word+((int8_t)(d[0]))];
        }
    };
    instructions[oc] = i;

    // LD IXL,A
    oc = {0,0xDD,0x006F};
    i = { 9, 9, 0, INST{ z->getRegisters()->IX.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD (IX+d),B
    oc = {0,0xDD,0x0070};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->BC.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (IX+d),C
    oc = {0,0xDD,0x0071};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // LD (IX+d),D
    oc = {0,0xDD,0x0072};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->DE.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (IX+d),E
    oc = {0,0xDD,0x0073};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->DE.bytes.low;
        }
    };
    instructions[oc] = i;

    // LD (IX+d),H
    oc = {0,0xDD,0x0074};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->HL.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (IX+d),L
    oc = {0,0xDD,0x0075};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->HL.bytes.low;
        }
    };
    instructions[oc] = i;

    // HALT
    oc = {0,0xDD,0x0076};
    i = { 4, 4, 0, INST{
            z->halt();
        }
    };
    instructions[oc] = i;

    // LD (IX+d),A
    oc = {0,0xDD,0x0077};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IX.word+((int8_t)d[0])] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD A,B
    oc = {0,0xDD,0x0078};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD A,C
    oc = {0,0xDD,0x0079};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD A,D
    oc = {0,0xDD,0x007A};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD A,E
    oc = {0,0xDD,0x007B};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD A,IXH
    oc = {0,0xDD,0x007C};
    i = { 9, 9, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->IX.bytes.high; } };
    instructions[oc] = i;

    // LD A,IXL
    oc = {0,0xDD,0x007D};
    i = { 9, 9, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->IX.bytes.low; } };
    instructions[oc] = i;

    // LD A,(IX+d)
    oc = {0,0xDD,0x007E};
    i = { 19, 19, 1, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->IX.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD A,A
    oc = {0,0xDD,0x007F};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // ADD A,B
    oc = {0,0xDD,0x0080};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,C
    oc = {0,0xDD,0x0081};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,D
    oc = {0,0xDD,0x0082};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,E
    oc = {0,0xDD,0x0083};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,IXH
    oc = {0,0xDD,0x0084};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IX.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,IXL
    oc = {0,0xDD,0x0085};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IX.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,(IX+d)
    oc = {0,0xDD,0x0086};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, (*m)[z->getRegisters()->IX.word+((int8_t)d[0])], r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,A
    oc = {0,0xDD,0x0087};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->AF.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,B
    oc = {0,0xDD,0x0088};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,C
    oc = {0,0xDD,0x0089};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,D
    oc = {0,0xDD,0x008A};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,E
    oc = {0,0xDD,0x008B};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,IXH
    oc = {0,0xDD,0x008C};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IX.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,IXL
    oc = {0,0xDD,0x008D};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IX.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,(IX+d)
    oc = {0,0xDD,0x008E};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, (*m)[z->getRegisters()->IX.word+((int8_t)d[0])], r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,A
    oc = {0,0xDD,0x008F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->AF.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // SUB B
    oc = {0,0xDD,0x0090};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB C
    oc = {0,0xDD,0x0091};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB D
    oc = {0,0xDD,0x0092};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB E
    oc = {0,0xDD,0x0093};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB IXH
    oc = {0,0xDD,0x0094};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IX.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB IXL
    oc = {0,0xDD,0x0095};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IX.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB (IX+d)
    oc = {0,0xDD,0x0096};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->IX.word+((int8_t)d[0])]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB A
    oc = {0,0xDD,0x0097};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,B  
    oc = {0,0xDD,0x0098};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,C
    oc = {0,0xDD,0x0099};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,D
    oc = {0,0xDD,0x009A};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,E
    oc = {0,0xDD,0x009B};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,IXH
    oc = {0,0xDD,0x009C};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IX.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,IXL
    oc = {0,0xDD,0x009D};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IX.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,(IX+d)
    oc = {0,0xDD,0x009E};
    i = { 19, 19, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->IX.word+((int8_t)d[0])]), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,A
    oc = {0,0xDD,0x009F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // AND B
    oc = {0,0xDD,0x00A0};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND C
    oc = {0,0xDD,0x00A1};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND D
    oc = {0,0xDD,0x00A2};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND E
    oc = {0,0xDD,0x00A3};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND IXH
    oc = {0,0xDD,0x00A4};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->IX.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND IXL
    oc = {0,0xDD,0x00A5};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->IX.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND (IX+d)
    oc = {0,0xDD,0x00A6};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, (*m)[z->getRegisters()->IX.word+((int8_t)d[0])], r);
        }
    };
    instructions[oc] = i;

    // AND A
    oc = {0,0xDD,0x00A7};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR B
    oc = {0,0xDD,0x00A8};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR C
    oc = {0,0xDD,0x00A9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR D
    oc = {0,0xDD,0x00AA};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR E
    oc = {0,0xDD,0x00AB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR IXH
    oc = {0,0xDD,0x00AC};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->IX.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR IXL
    oc = {0,0xDD,0x00AD};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->IX.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR (IX+d)
    oc = {0,0xDD,0x00AE};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, (*m)[z->getRegisters()->IX.word+((int8_t)d[0])], r);
        }
    };
    instructions[oc] = i;

    // XOR A
    oc = {0,0xDD,0x00AF};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR B
    oc = {0,0xDD,0x00B0};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR C
    oc = {0,0xDD,0x00B1};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR D
    oc = {0,0xDD,0x00B2};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR E
    oc = {0,0xDD,0x00B3};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR IXH
    oc = {0,0xDD,0x00B4};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->IX.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR IXL
    oc = {0,0xDD,0x00B5};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->IX.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR (IX+d)
    oc = {0,0xDD,0x00B6};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, (*m)[z->getRegisters()->IX.word+((int8_t)d[0])], r);
        }
    };
    instructions[oc] = i;

    // OR A
    oc = {0,0xDD,0x00B7};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // CP B
    oc = {0,0xDD,0x00B8};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP C
    oc = {0,0xDD,0x00B9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;            
        }
    };
    instructions[oc] = i;

    // CP D
    oc = {0,0xDD,0x00BA};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP E
    oc = {0,0xDD,0x00BB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP IXH
    oc = {0,0xDD,0x00BC};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->IX.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP IXL
    oc = {0,0xDD,0x00BD};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->IX.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP (IX+d)
    oc = {0,0xDD,0x00BE};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->IX.word+((int8_t)d[0])]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP A
    oc = {0,0xDD,0x00BF};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RET NZ
    oc = {0,0xDD,0x00C0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::NZ);
        }
    };
    instructions[oc] = i;

    // POP BC
    oc = {0,0xDD,0x00C1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP NZ,nn
    oc = {0,0xDD,0x00C2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.ZF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // JP nn
    oc = {0,0xDD,0x00C3};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            r->PC = nn;
        }
    };
    instructions[oc] = i;

    // CALL NZ,nn
    oc = {0,0xDD,0x00C4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::NZ, nn);
        }
    };
    instructions[oc] = i;

    // PUSH BC
    oc = {0,0xDD,0x00C5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->BC.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // ADD A,n
    oc = {0,0xDD,0x00C6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, d[0], r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // RST 00
    oc = {0,0xDD,0x00C7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x00;
        }
    };
    instructions[oc] = i;

    // RET Z
    oc = {0,0xDD,0x00C8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::Z);
        }
    };
    instructions[oc] = i;

    // RET
    oc = {0,0xDD,0x00C9};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = 0 | (*m)[r->SP];
            r->SP++;
            r->PC |= ((*m)[r->SP]) << 8;
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP Z,nn
    oc = {0,0xDD,0x00CA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.ZF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // 0x00CB is prefix

    // CALL Z,nn
    oc = {0,0xDD,0x00CC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::Z, nn);
        }
    };
    instructions[oc] = i;

    // CALL nn
    oc = {0,0xDD,0x00CD};
    i = { 17, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            r->SP--;
            (*m)[r->SP] = (r->PC) >> 8;
            r->SP--;
            (*m)[r->SP] = (r->PC) & 0xFF;
            r->PC = nn;
        }
    };
    instructions[oc] = i;

    // ADC A,n
    oc = {0,0xDD,0x00CE};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, d[0], r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // RST 08
    oc = {0,0xDD,0x00CF};  
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x08;
        }
    };
    instructions[oc] = i;

    // RET NC
    oc = {0,0xDD,0x00D0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::NC);
        }
    };
    instructions[oc] = i;

    // POP DE
    oc = {0,0xDD,0x00D1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP NC,nn
    oc = {0,0xDD,0x00D2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.CF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // OUT (n),A
    oc = {0,0xDD,0x00D3};
    i = { 11, 11, 1, INST{
            Z80Registers* r = z->getRegisters();
            (*(z->getIoPorts()))[d[0]] = r->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // CALL NC,nn
    oc = {0,0xDD,0x00D4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::NC, nn);
        }
    };
    instructions[oc] = i;

    // PUSH DE
    oc = {0,0xDD,0x00D5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->DE.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->DE.bytes.low;
        }
    };
    instructions[oc] = i;

    // SUB n
    oc = {0,0xDD,0x00D6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 10
    oc = {0,0xDD,0x00D7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x10;
        }
    };
    instructions[oc] = i;

    // RET C
    oc = {0,0xDD,0x00D8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::C);
        }
    };
    instructions[oc] = i;

    // EXX
    oc = {0,0xDD,0x00D9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->BC.word;
            r->BC.word = r->BCx.word;
            r->BCx.word = tmp;
            tmp = r->DE.word;
            r->DE.word = r->DEx.word;
            r->DEx.word = tmp;
            tmp = r->HL.word;
            r->HL.word = r->HLx.word;
            r->HLx.word = tmp;
        }
    };
    instructions[oc] = i;

    // JP C,nn
    oc = {0,0xDD,0x00DA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.CF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // IN A,(n)
    oc = {0,0xDD,0x00DB};
    i = { 11, 11, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = (*(z->getIoPorts()))[d[0]];
        }
    };
    instructions[oc] = i;

    // CALL C,nn
    oc = {0,0xDD,0x00DC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::C, nn);
        }
    };
    instructions[oc] = i;

    // 0xDD is prefix

    // SBC A,n
    oc = {0,0xDD,0x00DE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 18
    oc = {0,0xDD,0x00DF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x18;
        }
    };
    instructions[oc] = i;

    // RET PO
    oc = {0,0xDD,0x00E0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::PO);
        }
    };
    instructions[oc] = i;

    // POP IX
    oc = {0,0xDD,0x00E1};
    i = { 14, 14, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IX.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP PO,nn
    oc = {0,0xDD,0x00E2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.PF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EX (SP),IX
    oc = {0,0xDD,0x00E3};
    i = { 23, 23, 1, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t tmp = r->IX.bytes.low;
            r->IX.bytes.low = (*m)[r->SP];
            (*m)[r->SP] = tmp;
            tmp = r->IX.bytes.high;
            r->IX.bytes.high = (*m)[(r->SP)+1];
            (*m)[(r->SP)+1] = tmp;
        }
    };
    instructions[oc] = i;

    // CALL PO,nn
    oc = {0,0xDD,0x00E4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::PO, nn);
        }
    };
    instructions[oc] = i;

    // PUSH IX
    oc = {0,0xDD,0x00E5};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->IX.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->IX.bytes.low;
        }
    };
    instructions[oc] = i;

    // AND n
    oc = {0,0xDD,0x00E6};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 20
    oc = {0,0xDD,0x00E7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x20;
        }
    };
    instructions[oc] = i;

    // RET PE
    oc = {0,0xDD,0x00E8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::PE);
        }
    };
    instructions[oc] = i;

    // JP,(IX)
    oc = {0,0xDD,0x00E9};
    i = { 8, 8, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = (*m)[r->IX.word];
        }
    };
    instructions[oc] = i;

    // JP PE,nn
    oc = {0,0xDD,0x00EA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.PF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EX DE,HL
    oc = {0,0xDD,0x00EB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->DE.word;
            r->DE.word = r->HL.word;
            r->HL.word = tmp;
        }
    };
    instructions[oc] = i;

    // CALL PE,nn
    oc = {0,0xDD,0x00EC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::PE, nn);
        }
    };
    instructions[oc] = i;

    // 0xED is prefix

    // XOR n
    oc = {0,0xDD,0x00EE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 28
    oc = {0,0xDD,0x00EF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x28;
        }
    };
    instructions[oc] = i;

    // RET P
    oc = {0,0xDD,0x00F0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::P);
        }
    };
    instructions[oc] = i;

    // POP AF
    oc = {0,0xDD,0x00F1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP P,nn
    oc = {0,0xDD,0x00F2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.SF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // DI
    oc = {0,0xDD,0x00F3};
    i = { 4, 4, 1, INST{
            Z80Registers* r = z->getRegisters();
            z->setIFF1(false);
            z->setIFF2(false);
        }
    };
    instructions[oc] = i;

    // CALL P,nn
    oc = {0,0xDD,0x00F4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::P, nn);
        }
    };
    instructions[oc] = i;

    // PUSH AF
    oc = {0,0xDD,0x00F5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->AF.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->AF.bytes.low.byte;
        }
    };
    instructions[oc] = i;

    // OR n
    oc = {0,0xDD,0x00F6};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 30
    oc = {0,0xDD,0x00F7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x30;
        }
    };
    instructions[oc] = i;

    // RET M
    oc = {0,0xDD,0x00F8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::M);
        }
    };
    instructions[oc] = i;

    // LD SP,IX
    oc = {0,0xDD,0x00F9};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = r->IX.word;
        }
    };
    instructions[oc] = i;

    // JP M,nn
    oc = {0,0xDD,0x00FA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.SF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EI
    oc = {0,0xDD,0x00FB};
    i = { 4, 4, 1, INST{
            Z80Registers* r = z->getRegisters();
            z->setIFF1(true);
            z->setIFF2(true);
        }
    };
    instructions[oc] = i;

    // CALL M,nn
    oc = {0,0xDD,0x00FC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::M, nn);
        }
    };
    instructions[oc] = i;

    // 0xFD is prefix

    // CP n
    oc = {0,0xDD,0x00FE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 38
    oc = {0,0xDD,0x00FF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x38;
        }
    };
    instructions[oc] = i;

    // NOP
    oc = {0,0xFD,0x0000};
    i = { 4, 4, 0, INST{ ; } };
    instructions[oc] = i;

    // LD BC,nn
    oc = {0,0xFD,0x0001};
    i = { 10, 10, 2, INST{ z->getRegisters()->BC.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (BC),A
    oc = {0,0xFD,0x0002};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->BC.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC BC
    oc = {0,0xFD,0x0003};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = add<uint16_t>(r->BC.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC B
    oc = {0,0xFD,0x0004};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.high == 0x7F) ? true : false;
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC B
    oc = {0,0xFD,0x0005};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.high == 0x80) ? true : false;
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD B,n
    oc = {0,0xFD,0x0006};
    i = { 7, 7, 1, INST{ z->getRegisters()->BC.bytes.high = d[0]; } };
    instructions[oc] = i;

    // RLCA
    oc = {0,0xFD,0x0007};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = (bool)((r->AF.bytes.high >> 7) & 0x01);
            r->AF.bytes.high = rol<uint8_t>(r->AF.bytes.high);
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // EX AF, AF'
    oc = {0,0xFD,0x0008};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->AFx.word;
            r->AFx.word = r->AF.word;
            r->AF.word = tmp;
        }
    };
    instructions[oc] = i;

    // ADD IY,BC
    oc = {0,0xFD,0x0009};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = add(r->IY.word, r->BC.word, r, ADD16);
            setUndocumentedFlags(r->IY.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(BC)
    oc = {0,0xFD,0x000A};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->BC.word];
        }
    };
    instructions[oc] = i;

    // DEC BC
    oc = {0,0xFD,0x000B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = add<uint16_t>(r->BC.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC C
    oc = {0,0xFD,0x000C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.low == 0x7F) ? true : false;
            r->BC.bytes.low = add<uint8_t>(r->BC.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC C
    oc = {0,0xFD,0x000D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->BC.bytes.low == 0x80) ? true : false;
            r->BC.bytes.low = add<uint8_t>(r->BC.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD C,n
    oc = {0,0xFD,0x000E};
    i = { 7, 7, 1, INST{ z->getRegisters()->BC.bytes.low = d[0]; } };
    instructions[oc] = i;

    // RRCA
    oc = {0,0xFD,0x000F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = (bool)((r->AF.bytes.high) & 0x01);
            r->AF.bytes.high = ror<uint8_t>(r->AF.bytes.high);
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // DJNZ d
    oc = {0,0xFD,0x0010};
    i = { 8, 13, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.bytes.high = add<uint8_t>(r->BC.bytes.high, -1, r, 0);
            if (r->BC.bytes.high != 0)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD DE,nn
    oc = {0,0xFD,0x0011};
    i = { 10, 10, 2, INST{ z->getRegisters()->DE.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (DE),A
    oc = {0,0xFD,0x0012};
    i = { 7, 7, 0, INST{
            (*m)[z->getRegisters()->DE.word] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC DE
    oc = {0,0xFD,0x0013};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = add<uint16_t>(r->DE.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC D
    oc = {0,0xFD,0x0014};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.high == 0x7F) ? true : false;
            r->DE.bytes.high = add<uint8_t>(r->DE.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC D
    oc = {0,0xFD,0x0015};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.high == 0x80) ? true : false;
            r->DE.bytes.high = add<uint8_t>(r->DE.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD D,n
    oc = {0,0xFD,0x0016};
    i = { 7, 7, 1, INST{ z->getRegisters()->DE.bytes.high = d[0]; } };
    instructions[oc] = i;

    // RLA
    oc = {0,0xFD,0x0017};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = rolc(r->AF.bytes.high, r->AF.bytes.low.CF);
            r->AF.bytes.low.HF = false;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // JR d
    oc = {0,0xFD,0x0018};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->PC += d[0];
        }
    };
    instructions[oc] = i;

    // ADD IY,DE
    oc = {0,0xFD,0x0019};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = add(r->IY.word, r->DE.word, r, ADD16);
            setUndocumentedFlags(r->IY.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(DE)
    oc = {0,0xFD,0x001A};
    i = { 7, 7, 0, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->DE.word];
        }
    };
    instructions[oc] = i;

    // DEC DE
    oc = {0,0xFD,0x001B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = add<uint16_t>(r->DE.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC E
    oc = {0,0xFD,0x001C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.low == 0x7F) ? true : false;
            r->DE.bytes.low = add<uint8_t>(r->DE.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC E
    oc = {0,0xFD,0x001D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->DE.bytes.low == 0x80) ? true : false;
            r->DE.bytes.low = add<uint8_t>(r->DE.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD E,n
    oc = {0,0xFD,0x001E};
    i = { 7, 7, 1, INST{ z->getRegisters()->DE.bytes.low = d[0]; } };
    instructions[oc] = i;

    // RRA
    oc = {0,0xFD,0x001F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.CF = rorc(r->AF.bytes.high, r->AF.bytes.low.CF);
            r->AF.bytes.low.HF = false;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // JR NZ,d
    oc = {0,0xFD,0x0020};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.ZF == false)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD IYL,nn
    oc = {0,0xFD,0x0021};
    i = { 14, 14, 2, INST{ z->getRegisters()->IY.word = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (nn),IY
    oc = {0,0xFD,0x0022};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->IY.bytes.low;
            (*m)[nn+1] = z->getRegisters()->IY.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC IY
    oc = {0,0xFD,0x0023};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = add<uint16_t>(r->IY.word, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC IYH
    oc = {0,0xFD,0x0024};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IY.bytes.high == 0x7F) ? true : false;
            r->IY.bytes.high = add<uint8_t>(r->IY.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC IYH
    oc = {0,0xFD,0x0025};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IY.bytes.high == 0x80) ? true : false;
            r->IY.bytes.high = add<uint8_t>(r->IY.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD IYH,n
    oc = {0,0xFD,0x0026};
    i = { 9, 9, 1, INST{ z->getRegisters()->IY.bytes.high = d[0]; } };
    instructions[oc] = i;

    // DAA
    oc = {0,0xFD,0x0027};
    i = { 4, 4, 0, INST{ Z80Registers* r = z->getRegisters(); daa(r); } };
    instructions[oc] = i;

    // JR Z,d
    oc = {0,0xFD,0x0028};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.ZF == true)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // ADD IY,IY
    oc = {0,0xFD,0x0029};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = add(r->IY.word, r->IY.word, r, ADD16);
            setUndocumentedFlags(r->IY.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD IY,(nn)
    oc = {0,0xFD,0x002A};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            uint16_t in = CREATE_WORD((*m)[nn], (*m)[nn+1]);
            z->getRegisters()->IY.word = in;
        }
    };
    instructions[oc] = i;

    // DEC IY
    oc = {0,0xFD,0x002B};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = add<uint16_t>(r->IY.word, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC IYL
    oc = {0,0xFD,0x002C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IY.bytes.low == 0x7F) ? true : false;
            r->IY.bytes.low = add<uint8_t>(r->IY.bytes.low, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC IYL
    oc = {0,0xFD,0x002D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->IY.bytes.low == 0x80) ? true : false;
            r->IY.bytes.low = add<uint8_t>(r->IY.bytes.low, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD IYL,n
    oc = {0,0xFD,0x002E};
    i = { 7, 7, 1, INST{ z->getRegisters()->IY.bytes.low = d[0]; } };
    instructions[oc] = i;

    // CPL
    oc = {0,0xFD,0x002F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = ~(r->AF.bytes.high);
            r->AF.bytes.low.HF = true;
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // JR NC,d
    oc = {0,0xFD,0x0030};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.CF == false)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // LD SP,nn
    oc = {0,0xFD,0x0031};
    i = { 10, 10, 2, INST{ z->getRegisters()->SP = CREATE_WORD(d[0], d[1]); } };
    instructions[oc] = i;

    // LD (nn),A
    oc = {0,0xFD,0x0032};
    i = { 13, 13, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // INC SP
    oc = {0,0xFD,0x0033};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = add<uint16_t>(r->SP, 1, r, INC16);
        }
    };
    instructions[oc] = i;

    // INC (IY+d)
    oc = {0,0xFD,0x0034};
    i = { 23, 23, 1, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t byte = (*m)[r->IY.word+((int8_t)(d[0]))];
            r->AF.bytes.low.PF = (byte == 0x7F) ? true : false;
            byte = add<uint8_t>(byte, 1, r, INC8);
            (*m)[r->IY.word+((int8_t)(d[0]))] = byte;
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC (IY+d)
    oc = {0,0xFD,0x0035};
    i = { 23, 23, 1, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t byte = (*m)[r->IY.word+((int8_t)(d[0]))];
            r->AF.bytes.low.PF = (byte == 0x80) ? true : false;
            byte = add<uint8_t>(byte, -1, r, DEC8);
            (*m)[r->IY.word+((int8_t)(d[0]))] = byte;
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD (IY+d),n
    oc = {0,0xFD,0x0036};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = d[0];
        }
    };
    instructions[oc] = i;

    // SCF
    oc = {0,0xFD,0x0037};
    i = { 4, 4, 0, INST{
            z->getRegisters()->AF.bytes.low.CF = true;
            z->getRegisters()->AF.bytes.low.NF = false;
            z->getRegisters()->AF.bytes.low.HF = false;
        }
    };
    instructions[oc] = i;

    // JR C,d
    oc = {0,0xFD,0x0038};
    i = { 7, 12, 1, INST{
            Z80Registers* r = z->getRegisters();
            if (r->AF.bytes.low.CF == true)
            {
                r->PC += d[0];
            }
        }
    };
    instructions[oc] = i;

    // ADD IY,SP
    oc = {0,0xFD,0x0039};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = add(r->IY.word, r->SP, r, ADD16);
            setUndocumentedFlags(r->IY.word, r);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // LD A,(nn)
    oc = {0,0xFD,0x003A};
    i = { 13, 13, 0, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            z->getRegisters()->AF.bytes.high = (*m)[nn];
        }
    };
    instructions[oc] = i;

    // DEC SP
    oc = {0,0xFD,0x003B};
    i = { 6, 6, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = add<uint16_t>(r->SP, -1, r, DEC16);
        }
    };
    instructions[oc] = i;

    // INC A
    oc = {0,0xFD,0x003C};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->AF.bytes.high == 0x7F) ? true : false;
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, 1, r, INC8);
            r->AF.bytes.low.NF = false;
        }
    };
    instructions[oc] = i;

    // DEC A
    oc = {0,0xFD,0x003D};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.low.PF = (r->AF.bytes.high == 0x80) ? true : false;
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -1, r, DEC8);
            r->AF.bytes.low.NF = true;
        }
    };
    instructions[oc] = i;

    // LD A,n
    oc = {0,0xFD,0x003E};
    i = { 7, 7, 1, INST{ z->getRegisters()->AF.bytes.high = d[0]; } };
    instructions[oc] = i;

    // CCF
    oc = {0,0xFD,0x003F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            bool prevCarry = r->AF.bytes.low.CF;
            r->AF.bytes.low.CF = (r->AF.bytes.low.CF) ? false : true;
            r->AF.bytes.low.NF = false;
            r->AF.bytes.low.HF = prevCarry;
        }
    };
    instructions[oc] = i;

    // LD B,B
    oc = {0,0xFD,0x0040};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ }};
    instructions[oc] = i;

    // LD B,C
    oc = {0,0xFD,0x0041};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD B,D
    oc = {0,0xFD,0x0042};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD B,E
    oc = {0,0xFD,0x0043};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD B,IYH
    oc = {0,0xFD,0x0044};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->IY.bytes.high; } };
    instructions[oc] = i;

    // LD B,IYL
    oc = {0,0xFD,0x0045};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->IY.bytes.low; } };
    instructions[oc] = i;

    // LD B,(IY+d)
    oc = {0,0xFD,0x0046};
    i = { 19, 19, 1, INST{
            z->getRegisters()->BC.bytes.high = (*m)[z->getRegisters()->IY.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD B,A
    oc = {0,0xFD,0x0047};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD C,B
    oc = {0,0xFD,0x0048};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD C,C
    oc = {0,0xFD,0x0049};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD C,D
    oc = {0,0xFD,0x004A};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD C,E
    oc = {0,0xFD,0x004B};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD C,IYH
    oc = {0,0xFD,0x004C};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->IY.bytes.high; } };
    instructions[oc] = i;

    // LD C,IYL
    oc = {0,0xFD,0x004D};
    i = { 9, 9, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->IY.bytes.low; } };
    instructions[oc] = i;

    // LD C,(IY+d)
    oc = {0,0xFD,0x004E};
    i = { 7, 7, 1, INST{
            z->getRegisters()->BC.bytes.low = (*m)[z->getRegisters()->IY.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD C,A
    oc = {0,0xFD,0x004F};
    i = { 4, 4, 0, INST{ z->getRegisters()->BC.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD D,B
    oc = {0,0xFD,0x0050};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD D,C
    oc = {0,0xFD,0x0051};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD D,D
    oc = {0,0xFD,0x0052};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD D,E
    oc = {0,0xFD,0x0053};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD D,IYH
    oc = {0,0xFD,0x0054};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->IY.bytes.high; } };
    instructions[oc] = i;

    // LD D,IYL
    oc = {0,0xFD,0x0055};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->IY.bytes.low; } };
    instructions[oc] = i;

    // LD D,(IY+d)
    oc = {0,0xFD,0x0056};
    i = { 19, 19, 1, INST{
            z->getRegisters()->DE.bytes.high = (*m)[z->getRegisters()->IY.word+((int8_t)(d[0]))];
        }
    };
    instructions[oc] = i;

    // LD D,A
    oc = {0,0xFD,0x0057};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD E,B
    oc = {0,0xFD,0x0058};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD E,C
    oc = {0,0xFD,0x0059};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD E,D
    oc = {0,0xFD,0x005A};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD E,E
    oc = {0,0xFD,0x005B};
    i = { 4, 4, 0, INST{; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD E,IYH
    oc = {0,0xFD,0x005C};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->IY.bytes.high; } };
    instructions[oc] = i;

    // LD E,IYL
    oc = {0,0xFD,0x005D};
    i = { 9, 9, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->IY.bytes.low; } };
    instructions[oc] = i;

    // LD E,(IY+d)
    oc = {0,0xFD,0x005E};
    i = { 19, 19, 1, INST{
            z->getRegisters()->DE.bytes.low = (*m)[z->getRegisters()->IY.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD E,A
    oc = {0,0xFD,0x005F};
    i = { 4, 4, 0, INST{ z->getRegisters()->DE.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD IYH,B
    oc = {0,0xFD,0x0060};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD IYH,C
    oc = {0,0xFD,0x0061};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD IYH,D
    oc = {0,0xFD,0x0062};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD IYH,E
    oc = {0,0xFD,0x0063};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD IYH,IYH
    oc = {0,0xFD,0x0064};
    i = { 9, 9, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD IYH,IYL
    oc = {0,0xFD,0x0065};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.high = z->getRegisters()->IY.bytes.low; } };
    instructions[oc] = i;

    // LD H,(IY+d)
    oc = {0,0xFD,0x0066};
    i = { 19, 19, 1, INST{
            z->getRegisters()->HL.bytes.high = (*m)[z->getRegisters()->IY.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD IYH,A
    oc = {0,0xFD,0x0067};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.high = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD IYL,B
    oc = {0,0xFD,0x0068};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.low = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD IYL,C
    oc = {0,0xFD,0x0069};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.low = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD IYL,D
    oc = {0,0xFD,0x006A};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.low = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD IYL,E
    oc = {0,0xFD,0x006B};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.low = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD IYL,H
    oc = {0,0xFD,0x006C};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.low = z->getRegisters()->HL.bytes.high; } };
    instructions[oc] = i;

    // LD IYL,IYL
    oc = {0,0xFD,0x006D};
    i = { 9, 9, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // LD L,(IY+d)
    oc = {0,0xFD,0x006E};
    i = { 19, 19, 1, INST{
            z->getRegisters()->HL.bytes.low = (*m)[z->getRegisters()->IY.word+((int8_t)(d[0]))];
        }
    };
    instructions[oc] = i;

    // LD IYL,A
    oc = {0,0xFD,0x006F};
    i = { 9, 9, 0, INST{ z->getRegisters()->IY.bytes.low = z->getRegisters()->AF.bytes.high; } };
    instructions[oc] = i;

    // LD (IY+d),B
    oc = {0,0xFD,0x0070};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->BC.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (IY+d),C
    oc = {0,0xFD,0x0071};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // LD (IY+d),D
    oc = {0,0xFD,0x0072};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->DE.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (IY+d),E
    oc = {0,0xFD,0x0073};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->DE.bytes.low;
        }
    };
    instructions[oc] = i;

    // LD (IY+d),H
    oc = {0,0xFD,0x0074};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->HL.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD (IY+d),L
    oc = {0,0xFD,0x0075};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->HL.bytes.low;
        }
    };
    instructions[oc] = i;

    // HALT
    oc = {0,0xFD,0x0076};
    i = { 4, 4, 0, INST{
            z->halt();
        }
    };
    instructions[oc] = i;

    // LD (IY+d),A
    oc = {0,0xFD,0x0077};
    i = { 19, 19, 1, INST{
            (*m)[z->getRegisters()->IY.word+((int8_t)d[0])] = z->getRegisters()->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // LD A,B
    oc = {0,0xFD,0x0078};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->BC.bytes.high; } };
    instructions[oc] = i;

    // LD A,C
    oc = {0,0xFD,0x0079};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->BC.bytes.low; } };
    instructions[oc] = i;

    // LD A,D
    oc = {0,0xFD,0x007A};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->DE.bytes.high; } };
    instructions[oc] = i;

    // LD A,E
    oc = {0,0xFD,0x007B};
    i = { 4, 4, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->DE.bytes.low; } };
    instructions[oc] = i;

    // LD A,IYH
    oc = {0,0xFD,0x007C};
    i = { 9, 9, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->IY.bytes.high; } };
    instructions[oc] = i;

    // LD A,IYL
    oc = {0,0xFD,0x007D};
    i = { 9, 9, 0, INST{ z->getRegisters()->AF.bytes.high = z->getRegisters()->IY.bytes.low; } };
    instructions[oc] = i;

    // LD A,(IY+d)
    oc = {0,0xFD,0x007E};
    i = { 19, 19, 1, INST{
            z->getRegisters()->AF.bytes.high = (*m)[z->getRegisters()->IY.word+((int8_t)d[0])];
        }
    };
    instructions[oc] = i;

    // LD A,A
    oc = {0,0xFD,0x007F};
    i = { 4, 4, 0, INST{ ; /* Practically a NOP */ } };
    instructions[oc] = i;

    // ADD A,B
    oc = {0,0xFD,0x0080};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,C
    oc = {0,0xFD,0x0081};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,D
    oc = {0,0xFD,0x0082};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,E
    oc = {0,0xFD,0x0083};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,IYH
    oc = {0,0xFD,0x0084};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IY.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,IYL
    oc = {0,0xFD,0x0085};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IY.bytes.low, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,(IY+d)
    oc = {0,0xFD,0x0086};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, (*m)[z->getRegisters()->IY.word+((int8_t)d[0])], r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADD A,A
    oc = {0,0xFD,0x0087};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->AF.bytes.high, r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,B
    oc = {0,0xFD,0x0088};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,C
    oc = {0,0xFD,0x0089};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->BC.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,D
    oc = {0,0xFD,0x008A};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,E
    oc = {0,0xFD,0x008B};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->DE.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,IYH
    oc = {0,0xFD,0x008C};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IY.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,IYL
    oc = {0,0xFD,0x008D};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->IY.bytes.low, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,(IY+d)
    oc = {0,0xFD,0x008E};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, (*m)[z->getRegisters()->IY.word+((int8_t)d[0])], r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // ADC A,A
    oc = {0,0xFD,0x008F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, r->AF.bytes.high, r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // SUB B
    oc = {0,0xFD,0x0090};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB C
    oc = {0,0xFD,0x0091};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB D
    oc = {0,0xFD,0x0092};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB E
    oc = {0,0xFD,0x0093};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB IYH
    oc = {0,0xFD,0x0094};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IY.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB IYL
    oc = {0,0xFD,0x0095};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IY.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB (IY+d)
    oc = {0,0xFD,0x0096};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->IY.word+((int8_t)d[0])]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SUB A
    oc = {0,0xFD,0x0097};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,B  
    oc = {0,0xFD,0x0098};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,C
    oc = {0,0xFD,0x0099};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,D
    oc = {0,0xFD,0x009A};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,E
    oc = {0,0xFD,0x009B};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,IYH
    oc = {0,0xFD,0x009C};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IY.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,IYL
    oc = {0,0xFD,0x009D};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->IY.bytes.low), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,(IY+d)
    oc = {0,0xFD,0x009E};
    i = { 19, 19, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->IY.word+((int8_t)d[0])]), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // SBC A,A
    oc = {0,0xFD,0x009F};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // AND B
    oc = {0,0xFD,0x00A0};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND C
    oc = {0,0xFD,0x00A1};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND D
    oc = {0,0xFD,0x00A2};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND E
    oc = {0,0xFD,0x00A3};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND IYH
    oc = {0,0xFD,0x00A4};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->IY.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // AND IYL
    oc = {0,0xFD,0x00A5};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->IY.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // AND (IY+d)
    oc = {0,0xFD,0x00A6};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, (*m)[z->getRegisters()->IY.word+((int8_t)d[0])], r);
        }
    };
    instructions[oc] = i;

    // AND A
    oc = {0,0xFD,0x00A7};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR B
    oc = {0,0xFD,0x00A8};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR C
    oc = {0,0xFD,0x00A9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR D
    oc = {0,0xFD,0x00AA};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR E
    oc = {0,0xFD,0x00AB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR IYH
    oc = {0,0xFD,0x00AC};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->IY.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // XOR IYL
    oc = {0,0xFD,0x00AD};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->IY.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // XOR (IY+d)
    oc = {0,0xFD,0x00AE};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, (*m)[z->getRegisters()->IY.word+((int8_t)d[0])], r);
        }
    };
    instructions[oc] = i;

    // XOR A
    oc = {0,0xFD,0x00AF};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR B
    oc = {0,0xFD,0x00B0};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->BC.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR C
    oc = {0,0xFD,0x00B1};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->BC.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR D
    oc = {0,0xFD,0x00B2};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->DE.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR E
    oc = {0,0xFD,0x00B3};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->DE.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR IYH
    oc = {0,0xFD,0x00B4};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->IY.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // OR IYL
    oc = {0,0xFD,0x00B5};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->IY.bytes.low, r);
        }
    };
    instructions[oc] = i;

    // OR (IY+d)
    oc = {0,0xFD,0x00B6};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, (*m)[z->getRegisters()->IY.word+((int8_t)d[0])], r);
        }
    };
    instructions[oc] = i;

    // OR A
    oc = {0,0xFD,0x00B7};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, r->AF.bytes.high, r);
        }
    };
    instructions[oc] = i;

    // CP B
    oc = {0,0xFD,0x00B8};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP C
    oc = {0,0xFD,0x00B9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->BC.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;            
        }
    };
    instructions[oc] = i;

    // CP D
    oc = {0,0xFD,0x00BA};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP E
    oc = {0,0xFD,0x00BB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->DE.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP IYH
    oc = {0,0xFD,0x00BC};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->IY.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP IYL
    oc = {0,0xFD,0x00BD};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->IY.bytes.low), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP (IY+d)
    oc = {0,0xFD,0x00BE};
    i = { 19, 19, 1, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -((*m)[z->getRegisters()->IY.word+((int8_t)d[0])]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // CP A
    oc = {0,0xFD,0x00BF};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(r->AF.bytes.high), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RET NZ
    oc = {0,0xFD,0x00C0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::NZ);
        }
    };
    instructions[oc] = i;

    // POP BC
    oc = {0,0xFD,0x00C1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP NZ,nn
    oc = {0,0xFD,0x00C2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.ZF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // JP nn
    oc = {0,0xFD,0x00C3};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            r->PC = nn;
        }
    };
    instructions[oc] = i;

    // CALL NZ,nn
    oc = {0,0xFD,0x00C4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::NZ, nn);
        }
    };
    instructions[oc] = i;

    // PUSH BC
    oc = {0,0xFD,0x00C5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->BC.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // ADD A,n
    oc = {0,0xFD,0x00C6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, d[0], r, ADD8);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // RST 00
    oc = {0,0xFD,0x00C7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x00;
        }
    };
    instructions[oc] = i;

    // RET Z
    oc = {0,0xFD,0x00C8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::Z);
        }
    };
    instructions[oc] = i;

    // RET
    oc = {0,0xFD,0x00C9};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = 0 | (*m)[r->SP];
            r->SP++;
            r->PC |= ((*m)[r->SP]) << 8;
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP Z,nn
    oc = {0,0xFD,0x00CA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.ZF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // 0x00CB is prefix

    // CALL Z,nn
    oc = {0,0xFD,0x00CC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::Z, nn);
        }
    };
    instructions[oc] = i;

    // CALL nn
    oc = {0,0xFD,0x00CD};
    i = { 17, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            r->SP--;
            (*m)[r->SP] = (r->PC) >> 8;
            r->SP--;
            (*m)[r->SP] = (r->PC) & 0xFF;
            r->PC = nn;
        }
    };
    instructions[oc] = i;

    // ADC A,n
    oc = {0,0xFD,0x00CE};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add(r->AF.bytes.high, d[0], r, ADD8, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // RST 08
    oc = {0,0xFD,0x00CF};  
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x08;
        }
    };
    instructions[oc] = i;

    // RET NC
    oc = {0,0xFD,0x00D0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::NC);
        }
    };
    instructions[oc] = i;

    // POP DE
    oc = {0,0xFD,0x00D1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->DE.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP NC,nn
    oc = {0,0xFD,0x00D2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.CF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // OUT (n),A
    oc = {0,0xFD,0x00D3};
    i = { 11, 11, 1, INST{
            Z80Registers* r = z->getRegisters();
            (*(z->getIoPorts()))[d[0]] = r->AF.bytes.high;
        }
    };
    instructions[oc] = i;

    // CALL NC,nn
    oc = {0,0xFD,0x00D4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::NC, nn);
        }
    };
    instructions[oc] = i;

    // PUSH DE
    oc = {0,0xFD,0x00D5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->DE.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->DE.bytes.low;
        }
    };
    instructions[oc] = i;

    // SUB n
    oc = {0,0xFD,0x00D6};
    i = { 7, 7, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 10
    oc = {0,0xFD,0x00D7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x10;
        }
    };
    instructions[oc] = i;

    // RET C
    oc = {0,0xFD,0x00D8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::C);
        }
    };
    instructions[oc] = i;

    // EXX
    oc = {0,0xFD,0x00D9};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->BC.word;
            r->BC.word = r->BCx.word;
            r->BCx.word = tmp;
            tmp = r->DE.word;
            r->DE.word = r->DEx.word;
            r->DEx.word = tmp;
            tmp = r->HL.word;
            r->HL.word = r->HLx.word;
            r->HLx.word = tmp;
        }
    };
    instructions[oc] = i;

    // JP C,nn
    oc = {0,0xFD,0x00DA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.CF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // IN A,(n)
    oc = {0,0xFD,0x00DB};
    i = { 11, 11, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = (*(z->getIoPorts()))[d[0]];
        }
    };
    instructions[oc] = i;

    // CALL C,nn
    oc = {0,0xFD,0x00DC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::C, nn);
        }
    };
    instructions[oc] = i;

    // 0xFD is prefix

    // SBC A,n
    oc = {0,0xFD,0x00DE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 18
    oc = {0,0xFD,0x00DF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x18;
        }
    };
    instructions[oc] = i;

    // RET PO
    oc = {0,0xFD,0x00E0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::PO);
        }
    };
    instructions[oc] = i;

    // POP IY
    oc = {0,0xFD,0x00E1};
    i = { 14, 14, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IY.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP PO,nn
    oc = {0,0xFD,0x00E2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.PF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EX (SP),IY
    oc = {0,0xFD,0x00E3};
    i = { 23, 23, 1, INST{
            Z80Registers* r = z->getRegisters();
            uint8_t tmp = r->IY.bytes.low;
            r->IY.bytes.low = (*m)[r->SP];
            (*m)[r->SP] = tmp;
            tmp = r->IY.bytes.high;
            r->IY.bytes.high = (*m)[(r->SP)+1];
            (*m)[(r->SP)+1] = tmp;
        }
    };
    instructions[oc] = i;

    // CALL PO,nn
    oc = {0,0xFD,0x00E4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::PO, nn);
        }
    };
    instructions[oc] = i;

    // PUSH IY
    oc = {0,0xFD,0x00E5};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->IY.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->IY.bytes.low;
        }
    };
    instructions[oc] = i;

    // AND n
    oc = {0,0xFD,0x00E6};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = and(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 20
    oc = {0,0xFD,0x00E7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x20;
        }
    };
    instructions[oc] = i;

    // RET PE
    oc = {0,0xFD,0x00E8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::PE);
        }
    };
    instructions[oc] = i;

    // JP,(IY)
    oc = {0,0xFD,0x00E9};
    i = { 8, 8, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = (*m)[r->IY.word];
        }
    };
    instructions[oc] = i;

    // JP PE,nn
    oc = {0,0xFD,0x00EA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.PF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EX DE,HL
    oc = {0,0xFD,0x00EB};
    i = { 4, 4, 0, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t tmp = r->DE.word;
            r->DE.word = r->HL.word;
            r->HL.word = tmp;
        }
    };
    instructions[oc] = i;

    // CALL PE,nn
    oc = {0,0xFD,0x00EC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::PE, nn);
        }
    };
    instructions[oc] = i;

    // 0xED is prefix

    // XOR n
    oc = {0,0xFD,0x00EE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = xor(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 28
    oc = {0,0xFD,0x00EF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x28;
        }
    };
    instructions[oc] = i;

    // RET P
    oc = {0,0xFD,0x00F0};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::P);
        }
    };
    instructions[oc] = i;

    // POP AF
    oc = {0,0xFD,0x00F1};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.word = CREATE_WORD((*m)[(r->SP)++], (*m)[r->SP]);
            r->SP++;
        }
    };
    instructions[oc] = i;

    // JP P,nn
    oc = {0,0xFD,0x00F2};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (!(r->AF.bytes.low.SF))
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // DI
    oc = {0,0xFD,0x00F3};
    i = { 4, 4, 1, INST{
            Z80Registers* r = z->getRegisters();
            z->setIFF1(false);
            z->setIFF2(false);
        }
    };
    instructions[oc] = i;

    // CALL P,nn
    oc = {0,0xFD,0x00F4};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::P, nn);
        }
    };
    instructions[oc] = i;

    // PUSH AF
    oc = {0,0xFD,0x00F5};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->AF.bytes.high;
            r->SP--;
            (*m)[r->SP] = r->AF.bytes.low.byte;
        }
    };
    instructions[oc] = i;

    // OR n
    oc = {0,0xFD,0x00F6};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            r->AF.bytes.high = or(r->AF.bytes.high, d[0], r);
        }
    };
    instructions[oc] = i;

    // RST 30
    oc = {0,0xFD,0x00F7};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x30;
        }
    };
    instructions[oc] = i;

    // RET M
    oc = {0,0xFD,0x00F8};
    i = { 5, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            retc(r, m, RetCondition::M);
        }
    };
    instructions[oc] = i;

    // LD SP,IY
    oc = {0,0xFD,0x00F9};
    i = { 10, 10, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP = r->IY.word;
        }
    };
    instructions[oc] = i;

    // JP M,nn
    oc = {0,0xFD,0x00FA};
    i = { 10, 10, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            if (r->AF.bytes.low.SF)
            {
                r->PC = nn;
            }
        }
    };
    instructions[oc] = i;

    // EI
    oc = {0,0xFD,0x00FB};
    i = { 4, 4, 1, INST{
            Z80Registers* r = z->getRegisters();
            z->setIFF1(true);
            z->setIFF2(true);
        }
    };
    instructions[oc] = i;

    // CALL M,nn
    oc = {0,0xFD,0x00FC};
    i = { 10, 17, 2, INST{
            Z80Registers* r = z->getRegisters();
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            callc(r, m, RetCondition::M, nn);
        }
    };
    instructions[oc] = i;

    // 0xFD is prefix

    // CP n
    oc = {0,0xFD,0x00FE};
    i = { 7, 7, 1, INST{
            Z80Registers* r = z->getRegisters();
            add<uint8_t>(r->AF.bytes.high, -(d[0]), r, SUB8);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // RST 38
    oc = {0,0xFD,0x00FF};
    i = { 11, 11, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->SP--;
            (*m)[r->SP] = r->PC >> 8;
            r->SP--;
            (*m)[r->SP] = r->PC & 0xFF;
            r->PC = 0 | 0x38;
        }
    };
    instructions[oc] = i;

    /* 0xED00 - 0xED3F are NOPs */

    // IN B,(C)
    oc = {0,0xED,0x0040};
    i = { 12, 12, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.bytes.high = (*(z->getIoPorts()))[r->BC.bytes.low];
        }
    };
    instructions[oc] = i;

    // OUT (C),B
    oc = {0,0xED,0x0041};
    i = { 12, 12, 0, INST{
            Z80Registers* r = z->getRegisters();
            (*(z->getIoPorts()))[r->BC.bytes.low] = r->BC.bytes.high;
        }
    };
    instructions[oc] = i;

    // SBC HL,BC
    oc = {0,0xED,0x0042};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add<uint16_t>(r->HL.word, -(r->BC.word), r, SUB16, false, true);
            r->AF.bytes.low.NF = 1;
        }
    };
    instructions[oc] = i;

    // LD (nn),BC
    oc = {0,0xED,0x0043};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            (*m)[nn] = z->getRegisters()->BC.bytes.low;
            (*m)[nn+1] = z->getRegisters()->BC.bytes.high;
        }
    };
    instructions[oc] = i;

    // NEG
    oc = {0,0xED,0x0044};
    i = { 8, 8, 0, INST{
            Z80Registers* r = z->getRegisters();
            if ( r->AF.bytes.high == 0x80 ) { r->AF.bytes.low.PF = true; }
            else { r->AF.bytes.low.PF = false; }
            r->AF.bytes.low.CF = r->AF.bytes.high != 0x00;
            r->AF.bytes.high = add<uint8_t>(0, -(r->AF.bytes.high), r, NEG8);
            r->AF.bytes.low.NF = true;

        }
    };
    instructions[oc] = i;

    // RETN
    oc = {0,0xED,0x0045};
    i = { 14, 14, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->PC = 0;
            r->PC |= (*m)[r->SP];
            r->SP++;
            r->PC |= (*m)[r->SP] << 8;
            r->SP++;
            z->setIFF1(z->getIFF2());
        }
    };
    instructions[oc] = i;

    // IM 0
    oc = {0,0xED,0x0046};
    i = { 8, 8, 0, INST{
            z->setInterruptMode(0);
        }
    };
    instructions[oc] = i;

    // LD I,A
    oc = {0,0xED,0x0047};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IR.bytes.high = r->AF.bytes.high;            
        }
    };
    instructions[oc] = i;

    // IN C,(C)
    oc = {0,0xED,0x0048};
    i = { 12, 12, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->BC.bytes.low = (*(z->getIoPorts()))[r->BC.bytes.low];
        }
    };
    instructions[oc] = i;

    // OUT (C),C
    oc = {0,0xED,0x0049};
    i = { 12, 12, 0, INST{
            Z80Registers* r = z->getRegisters();
            (*(z->getIoPorts()))[r->BC.bytes.low] = r->BC.bytes.low;
        }
    };
    instructions[oc] = i;

    // ABC HL,BC
    oc = {0,0xED,0x004A};
    i = { 15, 15, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->HL.word = add<uint16_t>(r->HL.word, r->BC.word, r, ADD16, false, true);
            r->AF.bytes.low.NF = 0;
        }
    };
    instructions[oc] = i;

    // LD BC,(nn)
    oc = {0,0xED,0x004B};
    i = { 20, 20, 2, INST{
            uint16_t nn = CREATE_WORD(d[0], d[1]);
            z->getRegisters()->BC.bytes.low = (*m)[nn];
            z->getRegisters()->BC.bytes.high = (*m)[nn+1];
        }
    };
    instructions[oc] = i;

    // NEG
    oc = {0,0xED,0x004C};
    i = { 8, 8, 0, INST{
            Z80Registers* r = z->getRegisters();
            if ( r->AF.bytes.high == 0x80 ) { r->AF.bytes.low.PF = true; }
            else { r->AF.bytes.low.PF = false; }
            r->AF.bytes.low.CF = r->AF.bytes.high != 0x00;
            r->AF.bytes.high = add<uint8_t>(0, -(r->AF.bytes.high), r, NEG8);
            r->AF.bytes.low.NF = true;

        }
    };
    instructions[oc] = i;

    // RETI
    oc = {0,0xED,0x004D};
    i = { 14, 14, 0, INST{
            // Only differs from RETN in opcode
            Z80Registers* r = z->getRegisters();
            r->PC = 0;
            r->PC |= (*m)[r->SP];
            r->SP++;
            r->PC |= (*m)[r->SP] << 8;
            r->SP++;
            z->setIFF1(z->getIFF2());
        }
    };
    instructions[oc] = i;

    // IM 0
    oc = {0,0xED,0x004E};
    i = { 8, 8, 0, INST{
            z->setInterruptMode(0);
        }
    };
    instructions[oc] = i;

    // LD R,A
    oc = {0,0xED,0x004F};
    i = { 9, 9, 0, INST{
            Z80Registers* r = z->getRegisters();
            r->IR.bytes.low = r->AF.bytes.high;            
        }
    };
    instructions[oc] = i;

    return instructions;
}