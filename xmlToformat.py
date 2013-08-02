import xml.etree.ElementTree as etree
import os
import sys


if __name__ == "__main__":
    #os.chdir('E:\\CarData\\voc2007\\training\\car')
    fln=sys.argv[1]
    #fln='009938'
    tree = etree.parse('%s.xml'%(fln))
    
    objArray=[]
    for atype in tree.findall('object'):
        tArray=[]
        if atype.find('difficult').text=='0' and atype.find('name').text=='car':
            tArray.append(atype.find('bndbox').find('xmin').text)
            tArray.append(atype.find('bndbox').find('ymin').text)
            tArray.append(atype.find('bndbox').find('xmax').text)
            tArray.append(atype.find('bndbox').find('ymax').text)
            tArray.append(atype.find('truncated').text)
            tArray.append(atype.find('pose').text)
            objArray.append(tArray)
    
    fileposs=open(fln+'_objs.txt','w')
   
    fileposs.write('%d\n'%len(objArray))
    for ob in objArray:
        for sob in ob:
            fileposs.write('%s '%(sob))
        fileposs.write('\n')
    
    fileposs.close() 
