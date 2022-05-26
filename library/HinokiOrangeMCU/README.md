# HinokiOrangeMCU
MCU framework

### Main Example
```c
int main() {
    ho_process_init_after_reset();

    /** TODO: CodeGen initialize, platform init */

    ho_process_init();

    for(;;) {
        ho_process_routine();
    }

    return 0;
}
```

## Macros for Build
- `HO_CONFIG_HEADER`
    - type: string
    - default: undefined
    - additional header to be injected to config library
- `HO_DEBUG_MSG_LEN`
    - type: number
    - default: 128
    - Max length of logger

## Configuration

- Project
    - `HO_PROJ_NAME`
        - type: string
        - default: `"Project Name"`
        - Name of Project
    - `HO_PROJ_REV`
        - type: string
        - default: `"Project Revision"`
        - Revision of project, such as board version, spec version, ...
    - `HO_PROJ_VER_MAJOR`
        - type: number
        - default: `1`
        - Major version of project
    - `HO_PROJ_VER_MINOR`
        - type: number
        - default: `0`
        - Minor version of project
    - `HO_PROJ_VER_PATCH`
        - type: nubmer
        - default: `0`
        - Patch version of project
    - `HO_PROJ_VER_MOD`
        - type: string
        - default: `""`
        - Modification of version of project

