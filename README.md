# sysprog: a collection of systems programming exercises and snippets

Examples in this repository center around two texts:
+ Operating Systems: Three Easy Pieces by Remzi H. Arpaci-Dusseau & Andrea C.
  Arpaci-Dusseau
+ The Linux Programming Interface by Michael Kerrisk

The programs contained in this repo are copies of ones found in the books and
some exercises on similar topics. 

## .PHONY: compare
Make is used throughout the repository to build small example programs. 
In some cases, it's interesting and informative to view the assembly output by
the compiler with different optimization flags. In such examples, there will
often be a phony compare target that builds both versions and opens up a diff
in vim.


