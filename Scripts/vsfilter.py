import os
import sys
import uuid
from xml.etree import ElementTree

def indent(elem, level=0, hor='\t', ver='\n'):
    i = ver + level * hor
    if len(elem):
        if not elem.text or not elem.text.strip():
            elem.text = i + hor
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
        for elem in elem:
            indent(elem, level + 1, hor, ver)
        if not elem.tail or not elem.tail.strip():
            elem.tail = i
    else:
        if level and (not elem.tail or not elem.tail.strip()):
            elem.tail = i

if len(sys.argv) != 2:
    print('Error: invalid arguments.')
    sys.exit()

path = os.path.abspath(sys.argv[1])
folders = []
files = []

for root, dirs, filenames in os.walk(path):
    for filename in filenames:
        files.append(os.path.join(root, filename))
    for dirName in dirs:
        folders.append(os.path.join(root, dirName))

projectFile = ''
filterFile = ''
sources = []
headers = []
filters = []
for file in files:
    if file.endswith('.vcxproj'):
        projectFile = file
    if file.endswith('.vcxproj.filters'):
        filterFile = file
    if file.endswith('.cpp') or file.endswith('.c'):
        sources.append(os.path.relpath(file, path))
    if file.endswith('.hpp') or file.endswith('.h'):
        headers.append(os.path.relpath(file, path))

if projectFile == '' or filterFile == '':
    print('Error: could not find project')
    sys.exit()

# HARD CODED FOR NOW WILL FIX MUST FIX
# To filter out intermediate folders.
# Will eventually search in the vcxproj to find these folders instead
for folder in folders:
    if not (folder.endswith('x64') or folder.endswith('.tlog') or folder.endswith('Debug') or folder.endswith('Release')):
        filters.append(os.path.relpath(folder, path))

ElementTree.register_namespace('', 'http://schemas.microsoft.com/developer/msbuild/2003')
projectTree = ElementTree.parse(projectFile)
itemGroups = [itemGroup for itemGroup in projectTree.getroot().getchildren() if itemGroup.tag == '{http://schemas.microsoft.com/developer/msbuild/2003}ItemGroup' and len(itemGroup.attrib) == 0]

if len(itemGroups) != 2:
    print('Error: could not parse vcxproj file')
    sys.exit()

for itemGroup in itemGroups:
    itemGroup.clear()

# Add headers to vcxproj
for header in headers:
    clInclude = ElementTree.Element('ClInclude', { 'Include' : header})
    itemGroups[0].append(clInclude)

# Add sources to vcxproj
for source in sources:
    clCompile = ElementTree.Element('ClCompile', {'Include' : source})
    itemGroups[1].append(clCompile)

indent(projectTree.getroot())
projectTree.write(projectFile)

# Modify filter file
filterTree = ElementTree.parse(filterFile)
itemGroups = [itemGroup for itemGroup in filterTree.getroot().getchildren() if itemGroup.tag == '{http://schemas.microsoft.com/developer/msbuild/2003}ItemGroup']

for itemGroup in itemGroups:
    itemGroup.clear()

if len(itemGroups) != 3:
    print('Error: could not parse vcxproj.filters file')
    sys.exit()

for vcFilter in filters:
    filterElement = ElementTree.Element('Filter', { 'Include' : vcFilter })
    uuidElement = ElementTree.Element('UniqueIdentifier')
    uuidElement.text = '{' + str(uuid.uuid4()) + '}'
    filterElement.append(uuidElement)
    itemGroups[0].append(filterElement)

for header in headers:
    clInclude = ElementTree.Element('ClInclude', { 'Include' : header })
    filterElement = ElementTree.Element('Filter')
    filterElement.text = header[:header.rfind(os.sep)]
    clInclude.append(filterElement)
    itemGroups[1].append(clInclude)

for source in sources:
    clCompile = ElementTree.Element('ClCompile', { 'Include' : source })
    filterElement = ElementTree.Element('Filter')
    filterElement.text = source[:source.rfind(os.sep)]
    clCompile.append(filterElement)
    itemGroups[2].append(clCompile)

indent(filterTree.getroot())
filterTree.write(filterFile)