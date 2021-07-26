# Abandon all hope, ye who enter here

## Multi-level page tables 

### Step 1: Extract page-directory Index (PDIndex)

Find address of pae directory entry PDE
PDEAddr = PageDirBase + (PDIndex + sizeof(PDE))
