import xml.etree.ElementTree as ET
import json

tree = ET.parse('CMT02.xml')
root = tree.getroot()

print(root)

for child in root:
    print(child.tag, child.attrib)

Nodos = {}
Matriz_Distancia_Archivo = open("Matriz_Distancias.txt", "w")

for node in root.iter('node'):
    #print(node.attrib)
    #print(node.tag, node.attrib)
    print("Node: ")
    y = node.attrib
    x = json.dumps(y, indent=4)
    x = json.loads(x)
    print("ID_Node: ",x["id"])
    print("Type: ", x["type"])
    for cx in node:
        if cx.tag == "cx":
            CX = cx.text
            #print(cx.tag, cx.text)
        else:
            CY = cx.text
            #print(cx.tag, cx.text)
        #Nodos.update()
    
    Matriz_Distancia_Archivo.write(x["id"] + ".00, " + CX + "0, " + CY + "0, " + x["type"] +".00" +", \n")
    print("")


Archivo_vehicle_profile = open("Vehicle_Profile.txt", "w")

for node in root.iter('vehicle_profile'):
    #print(node.attrib)
    #print(node.tag, node.attrib)
    print("Vehicle: ")
    
    for profile in node:
        if profile.tag == "departure_node":
            departure_node = profile.text

        if profile.tag == "arrival_node":
            arrival_node = profile.text

        if profile.tag == "capacity":
            capacity = profile.text
        #print(profile.tag, profile.text)
        #if cx.tag == "cx":
            #CX = cx.text
            #print(cx.tag, cx.text)
        #else:
            #CY = cx.text
            #print(cx.tag, cx.text)
        #Nodos.update()
    
    Archivo_vehicle_profile.write(departure_node + ", " + arrival_node + ", " + capacity +"\n")
    print("")


Matriz_Demanda_Archivo = open("Demanda_Archivo.txt", "w")

for request in root.iter('request'):
    #print(node.attrib)
    #print(node.tag, node.attrib)
    print("Requests: ")
    y = request.attrib
    x = json.dumps(y, indent=4)
    x = json.loads(x)
    print("ID_Request: ",x["id"])
    print("Node: ", x["node"])
    for quantity in request:
        #print(quantity.text)
        quantity_value = quantity.text
    
    Matriz_Demanda_Archivo.write(x["id"] + ".00, " + x["node"] + ".00, " + quantity_value +"0, \n")
    print("")

Matriz_Distancia_Archivo.close()
Archivo_vehicle_profile.close()
Matriz_Demanda_Archivo.close()