# Redriver2PSP

Complete port of **Redriver2** (the reverse-engineered PC version of Driver 2 for PlayStation) to the **PlayStation Portable (PSP)**.

Consult the **[Porting Guide (PORTING_GUIDE.md)](PORTING_GUIDE.md)** and **[Roadmap (ROADMAP.md)](ROADMAP.md)** for detailed technical analysis, challenges, architecture, and instructions.

## Project Structure
- `src/`: Game source code (`Game/`, `PsyCross/`, `utils/`, and `platform/`).
- `sce_sys/`: PSP application metadata (`PARAM.SFO`, icons).
- `data/`: Game assets (`Driver 2` extracted disc data).
- `Makefile`: PSPSDK build system configuration.
