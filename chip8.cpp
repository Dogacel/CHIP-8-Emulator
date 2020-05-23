#include "chip8.h"
#include <cstdlib>
#include <iostream>

void CHIP8::init()
{
    REG_PC = 0x200;
    in = 0x0;
    REG_I = 0x0;
    REG_SP = 0x0;

    for (int i = 0; i < 80; ++i)
        MEM[i] = fontset[i];

    srand(time(NULL));
}

void CHIP8::tick()
{
    in = MEM[REG_PC + 1] | (MEM[REG_PC] << 8);
    // printf("0x%x\n", in);
    bool res = 0;
    unsigned char Vx = (in >> 8) & 0x000F;
    unsigned char Vy = (in >> 4) & 0x000F;
    switch (in >> 12)
    {
    case 0x0:
        if (in == 0x00E0)
        {
            // CLS
            cls();
        }
        else if (in == 0x00EE)
        {
            // RET
            REG_SP -= 1;
            REG_PC = STACK[REG_SP];
        }
        else
        {
            // JUMP to nnn
            printf("Error !");
            REG_PC = (in && 0xFFF) - 2;
        }
        break;
    case 0x1:
        // JP addr
        REG_PC = (in & 0x0FFF) - 2;
        break;
    case 0x2:
        // CALL addr
        STACK[REG_SP] = REG_PC;
        REG_SP += 1;
        REG_PC = (in & 0x0FFF) - 2;
        break;
    case 0x3:
        // SE Vx, byte
        if (REG_V[(in >> 8) & 0x000F] == (in & 0x00FF))
        {
            REG_PC += 2;
        }
        break;
    case 0x4:
        // SNE Vx, byte
        if (REG_V[(in >> 8) & 0x000F] != (in & 0x00FF))
        {
            REG_PC += 2;
        }

        break;
    case 0x5:
        // SE Vx, Vy
        if (REG_V[(in >> 8) & 0x000F] == REG_V[(in >> 4) & 0x000F])
        {
            REG_PC += 2;
        }
        break;
    case 0x6:
        // LD Vx, byte
        REG_V[(in >> 8) & 0x000F] = in & 0x00FF;
        break;
    case 0x7:
        // ADD Vx, byte
        REG_V[(in >> 8) & 0x000F] += in & 0x00FF;
        break;
    case 0x8:
        switch (in & 0x000F)
        {
        case 0x0:
            // LD Vx, Vy
            REG_V[Vx] = REG_V[Vy];
            break;
        case 0x1:
            // OR Vx, Vy
            REG_V[Vx] |= REG_V[Vy];
            break;
        case 0x2:
            // AND Vx, Vy
            REG_V[Vx] &= REG_V[Vy];
            break;
        case 0x3:
            // XOR Vx, Vy
            REG_V[Vx] ^= REG_V[Vy];
            break;
        case 0x4:
            // ADD Vx, Vy
            if (REG_V[Vx] > 0xFF - REG_V[Vy])
            {
                REG_V[0xF] = 1;
            }
            else
            {
                REG_V[0xF] = 0;
            }
            REG_V[Vx] = (REG_V[Vy] + REG_V[Vx]) & 0xFF;
            break;
        case 0x5:
            // SUB Vx, Vy
            REG_V[0xF] = REG_V[Vx] > REG_V[Vy] ? 1 : 0;
            REG_V[Vx] -= REG_V[Vy];
            break;
        case 0x6:
            // SHR Vx {, Vy}
            REG_V[0xF] = REG_V[Vx] & 0x1;
            REG_V[Vx] >>= 1;
            break;
        case 0x7:
            // SUBN Vx, Vy
            REG_V[0xF] = REG_V[Vy] > REG_V[Vx] ? 1 : 0;
            REG_V[Vx] = REG_V[Vy] - REG_V[Vx];
            break;
        case 0xE:
            // SHL Vx {, Vy}
            REG_V[0xF] = REG_V[Vx] >> 7;
            REG_V[Vx] <<= 1;
            break;
        }
        break;
    case 0x9:
        // SNE Vx, Vy
        if (REG_V[(in >> 8) & 0x000F] != REG_V[(in >> 4) & 0x000F])
        {
            REG_PC += 2;
        }
        break;
    case 0xA:
        // LD I, addr
        REG_I = in & 0x0FFF;
        break;
    case 0xB:
        // JP V0, addr
        REG_PC = (in & 0x0FFF) + REG_V[0x0] - 2;
        break;
    case 0xC:
        // RND Vx, byte
        // printf("RAND: %d\n", rand() & in & 0xFF);
        REG_V[Vx] = rand() & in & 0xFF;
        break;
    case 0xD:
        // DRW Vx, Vy, nibble
        // printf("Drawing %d to %d : %d\n", MEM[REG_I], REG_V[Vx], REG_V[Vy]);
        res = 0;
        for (unsigned char n = 0; n < (in & 0xF); n++)
        {
            res |= draw(REG_V[Vx], REG_V[Vy] + n, MEM[REG_I + n]);
        }
        if (res)
        {
            REG_V[0xF] = 1;
        }
        else
        {
            REG_V[0xF] = 0;
        }
        break;
    case 0xE:
        switch (in & 0xFF)
        {
        case 0x9E:
            // SKP Vx
            if (keys[REG_V[Vx]])
            {
                REG_PC += 2;
            }
            break;
        case 0xA1:
            // SKNP Vx
            if (!keys[REG_V[Vx]])
            {
                REG_PC += 2;
            }
            break;
        }
        break;
    case 0xF:
        switch (in & 0xFF)
        {
        case 0x07:
            // LD Vx, DT
            REG_V[Vx] = REG_DT / 8;
            break;
        case 0x0A:
            // LD Vx, K
            for (unsigned char i = 0; i < 0xF; i++)
            {
                if (keys[i])
                {
                    REG_PC += 2;
                    break;
                }
            }
            REG_PC -= 2;
            break;
        case 0x15:
            // LD DT, Vx
            REG_DT = REG_V[Vx] * 8;
            break;
        case 0x18:
            // LD ST, Vx
            REG_ST = REG_V[Vx];
            break;
        case 0x1E:
            // ADD I, Vx
            if (REG_I + REG_V[Vx] > 0xFFF)
            {
                REG_V[0xF] = 1;
            }
            else
            {
                REG_V[0xF] = 0;
            }
            REG_I += REG_V[Vx];
            break;
        case 0x29:
            // LD F, Vx
            REG_I = REG_V[Vx] * 5;
            break;
        case 0x33:
            // LD B, Vx
            MEM[REG_I] = REG_V[Vx] / 100;
            MEM[REG_I + 1] = (REG_V[Vx] % 100) / 10;
            MEM[REG_I + 2] = REG_V[Vx] % 10;
            break;
        case 0x55:
            // LD [I], Vx
            for (unsigned char i = 0x0; i <= Vx; i++)
            {
                MEM[REG_I + i] = REG_V[i];
            }
            break;
        case 0x65:
            //  LD Vx, [I]
            for (unsigned char i = 0x0; i <= Vx; i++)
            {
                REG_V[i] = MEM[REG_I + i];
            }
            break;
        }
        break;
    default:
        printf("Unknown instruction: 0x%X\n", in);
    }

    REG_PC += 2;

    if (REG_DT > 0)
    {
        REG_DT--;
    }

    if (REG_ST > 0)
    {
        if (REG_ST == 1)
        {
            printf("BEEP\n");
        }
        REG_ST--;
    }
}

bool CHIP8::draw(unsigned char x, unsigned char y, unsigned char sprite)
{
    bool retval = 0;
    while (sprite != 0x0)
    {
        if (gfx[x + y * 64] & (sprite >> 7) & 0x1)
        {
            retval = 1;
        }
        gfx[x + y * 64] ^= (sprite >> 7) & 0x1;
        sprite = sprite << 1;
        x++;
    }
    return retval;
}

void CHIP8::cls()
{
    for (unsigned short i = 0; i < 64 * 32; i++)
    {
        gfx[i] = 0x0;
    }
}

using namespace std;

void CHIP8::loadApp(const char *filename)
{

    FILE *pFile = fopen(filename, "rb");

    // Check file size
    fseek(pFile, 0, SEEK_END);
    long lSize = ftell(pFile);
    rewind(pFile);

    char *buffer = (char *)malloc(sizeof(char) * lSize);

    size_t result = fread(buffer, 1, lSize, pFile);

    for (size_t i = 0; i < result; i++)
    {
        MEM[i + 512] = buffer[i];
    }

    for (int i = 0; i < 128; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            printf("%02x ", MEM[j + i * 32]);
        }
        printf("\n");
    }
    printf("Loading complete!\n");
}
