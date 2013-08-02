import os
import sys

def infile(s):
    ins = open( s, "r" )
    array = []
    for line in ins:
        array.append( line.strip() )
    
    fls=array[1:int(array[0])+1]    
    return fls

if __name__ == "__main__":
    
    #os.chdir('E:\\CarData\\voc2007\\training\\car')
    fln=sys.argv[1]
    #fln='009269'
    thr=infile('..\\..\\bndThreshld.txt')
    stp=infile('..\\..\\kptStep.txt')
    
    bat=open('%stemrun.bat'%(fln),'wb')
    for th in thr:
        for st in stp:
            bat.write('start /B dimreduc %s_bt%s_stp%s \n'%(fln,th,st))
    bat.close()
    cmd='start /B %stemrun.bat'%(fln)
    print cmd 
    #os.system(cmd)
    