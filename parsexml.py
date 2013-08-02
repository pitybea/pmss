import xml.etree.ElementTree as etree
import os
import sys


if __name__ == "__main__":
    os.chdir('E:\\project\\pmsvoc')

    tree = etree.parse('000005.xml')
    for atype in tree.findall('object'):
        print atype.find('name').text
        
