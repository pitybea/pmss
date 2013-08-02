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
    ins=open(icd,'r')
    cds=[]
    
    for line in ins:
        cds.append(line.strip())
    
    ins.close()
    ecmd=split_list(cds,inm)
    
    allins=open('allcmds%d'%(inm),'wb')
    
    for i in range(0,len(ecmd)):
        ins=open('mycmd%d.bat'%(i),'wb')
        for acmd in ecmd[i]:
            ins.write('%s\n'%(acmd))
        
        ins.close()
        
        allins.write('start mycmd%d.bat \n'%(i))
    
    allins.close()
        
    