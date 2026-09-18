To install Sabre {{version}}:

```shell
# To install for Linux / Darwin
curl -fsSL https://sabre.rroessler.io/install.sh | bash --tag {{version}}

# Or alternatively on Windows
powershell -c "irm https://sabre.rroessler.io/install.ps1 | iex -Tag {{version}}"
```

To upgrade to Sabre {{version}}:

```shell
sabre upgrade --tag {{version}}
```
