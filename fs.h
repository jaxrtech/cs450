// On-disk file system format.
// Both the kernel and user programs use this header file.


#define ROOTINO 1  // root i-number
#define BSIZE 512  // block size

// Disk layout:
// [ boot block | super block | log | inode blocks |
//                                          free bit map | data blocks]
//
// mkfs computes the super block and builds an initial file system. The
// super block describes the disk layout:
struct superblock {
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};

#define NUM_PTRS_PER_BLOCK (BSIZE / sizeof(uint))

#define NUM_DIRECT_PTRS  10

#define NUM_SINGLE_INDIRECT_PTRS 2
#define NUM_SINGLE_INDIRECT_BLKS (NUM_SINGLE_INDIRECT_PTRS * NUM_PTRS_PER_BLOCK)
#define INDIRECT_TO_KADDR(BN) ((BN) / NUM_PTRS_PER_BLOCK)
#define INDIRECT_TO_BN(BN) ((BN) % NUM_PTRS_PER_BLOCK)

#define NUM_DOUBLE_INDIRECT_PTRS 1
#define NUM_DOUBLE_INDIRECT_BLKS (NUM_DOUBLE_INDIRECT_PTRS * NUM_PTRS_PER_BLOCK * NUM_PTRS_PER_BLOCK)
#define DINDIRECT_TO_KPTR(BN) ((BN) / (NUM_DOUBLE_INDIRECT_BLKS / NUM_DOUBLE_INDIRECT_PTRS))
#define DINDIRECT_TO_INDIRECT1_KPRT(BN) ((BN) / (NUM_DOUBLE_INDIRECT_BLKS / NUM_PTRS_PER_BLOCK))
#define DINDIRECT_TO_INDIRECT2_KPTR(BN) ((BN) % (NUM_DOUBLE_INDIRECT_BLKS / NUM_PTRS_PER_BLOCK))

#define NUM_INODE_PTRS    (NUM_DIRECT_PTRS + NUM_SINGLE_INDIRECT_PTRS + NUM_DOUBLE_INDIRECT_PTRS)

#define MAXFILE  ((NUM_DIRECT_PTRS) + (NUM_SINGLE_INDIRECT_BLKS) + (NUM_DOUBLE_INDIRECT_BLKS))

// On-disk inode structure
struct dinode {
  short type;           // File type
  short major;          // Major device number (T_DEV only)
  short minor;          // Minor device number (T_DEV only)
  short nlink;          // Number of links to inode in file system
  uint size;            // Size of file (bytes)
  uint addrs[NUM_INODE_PTRS];   // Data block addresses
};

// Inodes per block.
#define IPB           (BSIZE / sizeof(struct dinode))

// Block containing inode i
#define IBLOCK(i, sb)     ((i) / IPB + sb.inodestart)

// Bitmap bits per block
#define BPB           (BSIZE*8)

// Block of free map containing bit for block b
#define BBLOCK(b, sb) (b/BPB + sb.bmapstart)

// Directory is a file containing a sequence of dirent structures.
#define DIRSIZ 14

struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

