import os
import sys
if __name__ == "__main__":
    fln=sys.argv[1]
    fl=open('%s_num.txt'%(fln),'r')
    print fl.readline()
    fl.close()
    