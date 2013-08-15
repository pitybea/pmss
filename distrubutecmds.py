import os
import sys

def split_list(alist, wanted_parts=1):
    length = len(alist)
    return [ alist[i*length/wanted_parts: (i+1)*length/wanted_parts] 
             for i in range(wanted_parts) ]

if __name__ == "__main__":
#    os.chdir('E:\\TunnelHough')
    
    icd=sys.argv[1]
    inm=int(sys.argv[2])
    ex=sys.argv[3]
    ins=open(icd,'r')
    cds=[]
    
    for line in ins:
        cds.append(line.strip())
    
    ins.close()
    ecmd=split_list(cds,inm)
    
    allins=open('allcmds%s%d'%(ex,inm),'wb')
    
    for i in range(0,len(ecmd)):
        ins=open('mycmd%s%d.bat'%(ex,i),'wb')
        for acmd in ecmd[i]:
            ins.write('%s\n'%(acmd))
        
        ins.close()
        
        allins.write('start \B mycmd%s%d.bat \n'%(ex,i))
    
    allins.close()
        
    