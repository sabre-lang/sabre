---
icon: Upload
title: Upgrade
description: Always be up-to-date with the latest binary version
---

## Basic Usage

```shell title="Terminal"
sabre upgrade [options...]
```

This subcommand allows upgrading Sabre to a specified or latest version available.

## Common Options

### `--dry`

The `--dry` flag forces the upgrade handler to perform all actions without actually upgrading.

### `--tag`

By specifying the `--tag` option, Sabre will try to upgrade (or downgrade) to the desired version given. When not specified, it will attempt upgrading to the `latest` version available.

### `--force`

If the requested version is the same as the current installation, upgrading will not proceed. This can be circumvented by enabling the `--force` flag which will allow reinstalling the current version.

### `--quiet`

Disables verbose output whilst upgrading the runtime.

## Uninstalling Sabre

Sabre also allows uninstalling itself through the `--remove` flag. This flag will short-circuit all the others and force the runtime to self-destruct.

```shell title="Terminal"
# This will uninstall the executable and all native crates.
sabre upgrade --remove
```

Alternatively, the installation directory `$HOME/.sabre` can be manually deleted.
