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
    exs=sys.argv[3:len(sys.argv)]
    ins=open(icd,'r')
    cds=[]
    
    for line in ins:
        cds.append(line.strip())
    
    ins.close()
    ecmd=split_list(cds,inm)
    

    everystring="everycmd"
    
    allins=open('allcmds%s%d.bat'%(ex,inm),'wb')
    
    for i in range(0,len(ecmd)):
        ins=open('%s%s%d.bat'%(everystring,ex,i),'wb')
        for acmd in ecmd[i]:
            for extt in exs:
                ins.write('%s %s\n'%(extt,acmd))
        
        ins.close()
        
        allins.write('start /B %s%s%d.bat \n'%(everystring,ex,i))
    
    allins.close()
        
    