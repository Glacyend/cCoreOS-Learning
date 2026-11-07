"""Generate kernel/embed_apps.S from user/build/bin/*.bin.

Each app is embedded with `.incbin`. The `_num_app` table has the layout:
[num_app, app_0_start, app_1_start, ..., app_{n - 1}_start, app_{n - 1}_end]
"""

import os
import sys

USER_BIN_DIR = "user/build/bin"
OUTPUT = "kernel/embed_apps.S"


def main() -> int:
    if not os.path.isdir(USER_BIN_DIR):
        print(f"[gen_apps] {USER_BIN_DIR} not found: writing empty table.")
        apps = []
    else:
        apps = sorted(
            name[:-4] for name in os.listdir(USER_BIN_DIR) if name.endswith(".bin")
        )

    with open(OUTPUT, "w") as f:
        print(
            "    .align 3\n    .section .data\n    .globl _num_app\n_num_app:", file=f
        )
        print(f"    .quad {len(apps)}", file=f)
        for app in apps:
            print(f"    .quad app_{app}_start", file=f)
        if len(apps) != 0:
            print(f"    .quad app_{apps[-1]}_end", file=f)
        print(file=f)

        for app in apps:
            print("    .section .data", file=f)
            print(f"    .globl app_{app}_start", file=f)
            print(f"    .globl app_{app}_end", file=f)
            print(f"app_{app}_start:", file=f)
            print(f'    .incbin "../{USER_BIN_DIR}/{app}.bin"', file=f)
            print(f"app_{app}_end:", file=f)
            print(file=f)

    print(f"[gen_apps] Embedded {len(apps)} app(s) into {OUTPUT}.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
