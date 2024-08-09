SVC99A   TITLE 'SVC99A  - SVC99 services'
         SPACE 1
SVC99A   ASDSECT
SVC99A   CSECT
         YREGS

**| S99A..... SVC99
**| https://tech.mikefulton.ca/SVC99
**| Input:
**|   R1 -> pointer to S99RBP
**| Output:
**|   R15 -> RC 0 if successful, non-zero otherwise

SVC99A   CSECT
         ENTRY S99A
S99A     ASDPRO BASE_REG=3,USR_DSAL=S99A_DSAL

* Ensure the High Order Bit is ON for 0(R1)
         L   R2,0(,R1)
         OILH R2,X'8000'
         ST  R2,0(,R1)
* Call SVC99 (DYNALLOC) with S99RBP
         SVC 99
*
S99A_EXIT   DS    0H
         ASDEPI

         DROP
         LTORG

S99A_PARMS   DSECT
S99ARBP   DS AL4
S99A_DSAL EQU 0         

**| S99MSGA..... SVC99MSG
**| https://tech.mikefulton.ca/IEFDB476
**| Input:
**|   R1 -> pointer to em_parms
**| Output:
**|   R15 -> RC 0 if successful, non-zero otherwise

SVC99A   CSECT
         ENTRY S99MSGA
S99MSGA  ASDPRO BASE_REG=3,USR_DSAL=S99MSGA_DSAL
         USING S99MSGA_PARMS,R13

* Call SVC99MSG 
         LINK EP=IEFDB476
*
S99MSGA_EXIT   DS    0H
         ASDEPI

         DROP
         LTORG

S99MSGA_PARMS   DSECT
S99MSGAP DS  AL4
S99MSGA_DSAL EQU 0

**| Finish off the CSECT

SVC99A   CSECT
         DC    C'Open Source'
         END
