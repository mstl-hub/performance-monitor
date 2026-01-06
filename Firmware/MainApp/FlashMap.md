# Flash Memory Map

## Flash Layout

| Address | Description | Size | Sector |
|---------|-------------|------|--------|
| 0x8000000 | Prebootloader | 4 KB | 0 |
| 0x8001000 | Bootloader0 | 32 KB | 2 |
| 0x8009000 | Main Application | 256 KB | 18 |
| | Bootloader for rewritnig main bootloader | 32 KB | 173 |
| | Block error journal | 10 KB | 243 |
| | Block logs (auth, settings) | 10 KB | 248 |
| 0x807E800 | Main application config | 2 KB | 253 |
| 0x807F000 | Bootloader journal | 2 KB | 254 |
| 0x807F800 | Bootloader config | 2 KB | 255 |
| 0x8080000 | Reserve application | 256 KB | 256 |
| 0x80C0000 | (Unused) | | 384 |

### Total Flash Usage

- **Total Size**: 1024 KB
- **Usage**: Calculate from summed sector sizes

---

## Backup Power Register (BPR) Layout

| Register | Description | Size | Persistent | Name |
|----------|-------------|------|------------|-----------|
| BPR_DATA1 | BPR config version | 2 Bytes | ✓ | |
| BPR_DATA2 | State machine (boot sequence state) | 2 Bytes | ✓ | |
| BPR_DATA3 | | 0 Bytes | | |
| BPR_DATA4 | System data | 2 Bytes | ✓ | |
| BPR_DATA5 | Calendar (0) | 2 Bytes | ✓ | |
| BPR_DATA6 | Calendar (1) | 2 Bytes | ✓ | |
| BPR_DATA7 | (Reserved) | 0 Bytes | | |
| BPR_DATA8 | (Reserved) | 0 Bytes | | |
| BPR_DATA9 | (Reserved) | 0 Bytes | | |
| BPR_DATA10 | (Reserved) | 0 Bytes | | |
| BPR_DATA11 | (Reserved) | 0 Bytes | | |
| BPR_DATA12 | (Reserved) | 0 Bytes | | |
| BPR_DATA13 | (Reserved) | 0 Bytes | | |
| BPR_DATA14 | (Reserved) | 0 Bytes | | |
| BPR_DATA15 | (Reserved) | 0 Bytes | | |
| BPR_DATA16 | (Reserved) | 0 Bytes | | |
| BPR_DATA17 | (Reserved) | 0 Bytes | | |
| BPR_DATA18 | (Reserved) | 0 Bytes | | |
| BPR_DATA19 | (Reserved) | 0 Bytes | | |
| BPR_DATA20 | (Reserved) | 0 Bytes | | |
| BPR_DATA21 | (Reserved) | 0 Bytes | | |
| BPR_DATA22 | (Reserved) | 0 Bytes | | |
| BPR_DATA23 | (Reserved) | 0 Bytes | | |
| BPR_DATA24 | (Reserved) | 0 Bytes | | |
| BPR_DATA25 | (Reserved) | 0 Bytes | | |
| BPR_DATA26 | (Reserved) | 0 Bytes | | |
| BPR_DATA27 | (Reserved) | 0 Bytes | | |
| BPR_DATA28 | (Reserved) | 0 Bytes | | |
| BPR_DATA29 | System errors | 2 Bytes | | |
| BPR_DATA30 | Battery voltage V_BAT_2 reading (0) | 0 Bytes | | |
| BPR_DATA31 | Battery voltage V_BAT_2 reading (1) | | | |
| BPR_DATA32 | Battery voltage V_BAT reading (0) | 0 Bytes | | |
| BPR_DATA33 | Battery voltage V_BAT reading (0) | | | |
| BPR_DATA34 | Measurement counter | 0 Bytes | | |
| BPR_DATA35 | Sum of top temperatures (0) | 0 Bytes | | |
| BPR_DATA36 | Sum of top temperatures (1) | | | |
| BPR_DATA37 | Sum of top temperatures (2) | | | |
| BPR_DATA38 | Sum of top temperatures (3) | | | |
| BPR_DATA39 | Sum of main temperatures (0) | 0 Bytes | | |
| BPR_DATA40 | Sum of main temperatures (1) | | | |
| BPR_DATA41 | Sum of main temperatures (2) | | | |
| BPR_DATA42 | Sum of main temperatures (3) | | | |

### BPR Summary

- **Total Size**: 84 Bytes

---

## Notes

### Flash Sectors

- Sector 0: Prebootloader entry point
- Sectors 2-17: Bootloader0 code
- Sectors 18-172: Main application code
- Sectors 173+: Configuration and data storage
- Sectors 253-255: Critical system information

### BPR (Backup Power Registers)

- Accessible even in low-power modes
- Persists across brownout resets
- Used for boot sequence tracking and status
- CRC-protected for data integrity
- Total capacity: 84 bytes across BPR_DATA1-42

### Configuration Storage

- Sector 253 (0x807E800): Primary configuration
- Sector 254 (0x807F000): Error information backup
- Sector 255 (0x807F800): Bootloader status
