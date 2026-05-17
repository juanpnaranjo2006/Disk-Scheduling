import random
from sys import argv
import matplotlib.pyplot as plt

N_REQUESTS = 1000
N_CYLINDERS = 5000

def generateRandRequests(infLimit : int, supLimit : int, nRequests : int):
    return random.sample(range(infLimit, supLimit), nRequests)

def elementsOrder(a, b, reverse):
    ans = True
    if not reverse:
        ans = a > b
    else:
        ans = a < b
    return ans

def deleteRepetitions(requests : list[int]):
    i = 1
    while i < len(requests):
        if requests[i] == requests[i-1]:
            requests.pop(i)
        else:
            i += 1

def scanOrder(diskHeadInitialPos : int, requests : list[int], nCylinders : int, upwardsInitialDirection = True):
    orderedRequests = []

    for req in requests:
        if req == diskHeadInitialPos or elementsOrder(req, diskHeadInitialPos, not upwardsInitialDirection):
            orderedRequests.append(req)
    orderedRequests.sort(reverse=not upwardsInitialDirection)
    
    if len(orderedRequests) < len(requests):
        altRequests = []

        if upwardsInitialDirection and orderedRequests[len(orderedRequests) - 1] != (nCylinders - 1):
            orderedRequests.append(nCylinders - 1)
        elif not upwardsInitialDirection and orderedRequests[len(orderedRequests) - 1] != 0:
            orderedRequests.append(0)
        for req in requests:
            if elementsOrder(req, diskHeadInitialPos, upwardsInitialDirection):
                altRequests.append(req)
        altRequests.sort(reverse=upwardsInitialDirection)

        orderedRequests += altRequests
    
    orderedRequests.insert(0, diskHeadInitialPos)
    deleteRepetitions(orderedRequests)
    return orderedRequests

def c_scanOrder(diskHeadInitialPos : int, requests : list[int], nCylinders : int, upwardsInitialDirection = True):
    orderedRequests = []

    for req in requests:
        if req == diskHeadInitialPos or elementsOrder(req, diskHeadInitialPos, not upwardsInitialDirection):
            orderedRequests.append(req)
    orderedRequests.sort(reverse=not upwardsInitialDirection)

    if len(orderedRequests) < len(requests):
        altRequests = []

        if upwardsInitialDirection and orderedRequests[len(orderedRequests) - 1] != (nCylinders - 1):
            orderedRequests.append(nCylinders - 1)
            orderedRequests.append(0)
        elif upwardsInitialDirection:
            orderedRequests.append(0)
        elif not upwardsInitialDirection and orderedRequests[len(orderedRequests) - 1] != 0:
            orderedRequests.append(0)
            orderedRequests.append(nCylinders - 1)
        elif not upwardsInitialDirection:
            orderedRequests.append(nCylinders - 1)

        for req in requests:
            if elementsOrder(req, diskHeadInitialPos, upwardsInitialDirection):
                altRequests.append(req)
        altRequests.sort(reverse=not upwardsInitialDirection)

        orderedRequests += altRequests
    
    orderedRequests.insert(0, diskHeadInitialPos)
    deleteRepetitions(orderedRequests)
    return orderedRequests

def totalHeadMovement(requests : list[int]):
    totalMovement = 0
    if len(requests) > 0:
        prevPos = requests[0]
        for req in requests:
            totalMovement += abs(req - prevPos)
            prevPos = req
    return totalMovement

#Esta función no
def graficar_requests(ax, requests, titulo, max_y):
    x = list(range(len(requests)))
    y = requests

    # Dibujar puntos
    ax.scatter(x, y)

    # Dibujar flechas
    for i in range(len(requests) - 1):
        ax.annotate(
            '',
            xy=(x[i + 1], y[i + 1]),
            xytext=(x[i], y[i]),
            arrowprops=dict(arrowstyle='->')
        )

    ax.set_title(titulo)
    ax.set_xlabel("Orden de atención")
    ax.set_ylabel("Cilindro")

    # Mismo rango vertical para todos
    ax.set_ylim(0, max_y)

    ax.grid(True)

def main():
    parameters = argv
    if len(parameters) <= 1:
        print(f"Missing arguments! Provide the initial position for the disk head as a parameter in the command line")
    else:
        diskHeadInitialPos = int(parameters[1])
        if diskHeadInitialPos < 0 or diskHeadInitialPos >= N_CYLINDERS:
            print(f"Invalid Initial Position for Disk Head!\nInitial Cylinder: 0\nMaximum Cylinder: {N_CYLINDERS}\nDisk Head Initial Position: {diskHeadInitialPos}")
            return 0
        requests = generateRandRequests(0, N_CYLINDERS - 1, N_REQUESTS)
        print("Requests:")
        print(requests)

        scanRequestsUP = scanOrder(diskHeadInitialPos, requests, N_CYLINDERS)
        scanRequestsDOWN = scanOrder(diskHeadInitialPos, requests, N_CYLINDERS, False)
        c_scanRequestsUP = c_scanOrder(diskHeadInitialPos, requests, N_CYLINDERS)
        c_scanRequestsDOWN = c_scanOrder(diskHeadInitialPos, requests, N_CYLINDERS, False)
        totalMovScanUP = totalHeadMovement(scanRequestsUP)
        totalMovScanDOWN = totalHeadMovement(scanRequestsDOWN)
        totalMovC_ScanUP = totalHeadMovement(c_scanRequestsUP)
        totalMovC_ScanDOWN = totalHeadMovement(c_scanRequestsDOWN)

        #Hasta aquí

        #print("Algorithms:")

        #print("SCAN UP")
        #print(f"{scanRequestsUP} : Total Mov: {totalMovScanUP}")

        #print("SCAN DOWN")
        #print(f"{scanRequestsDOWN} : Total Mov: {totalMovScanDOWN}")

        #print("C-SCAN UP")
        #print(f"{c_scanRequestsUP} : Total Mov: {totalMovC_ScanUP}")

        #print("C-SCAN DOWN")
        #print(f"{c_scanRequestsDOWN} : Total Mov: {totalMovC_ScanDOWN}")

        # Máximo cilindro global
        max_y = max(max(requests), max(scanRequestsUP), max(scanRequestsDOWN), max(c_scanRequestsUP), max(c_scanRequestsDOWN))

        # Crear ventana con 5 gráficas
        fig, axs = plt.subplots(3, 2, figsize=(15, 12))

        # Convertir matriz de ejes en lista plana
        axs = axs.flatten()

        # FCFS
        graficar_requests(axs[0], requests, "FCFS", max_y)

        # SCAN
        graficar_requests(axs[1], scanRequestsUP, "SCAN UP", max_y)
        graficar_requests(axs[2], scanRequestsDOWN, "SCAN DOWN", max_y)

        # C-SCAN
        graficar_requests(axs[3], c_scanRequestsUP, "C-SCAN UP", max_y)
        graficar_requests(axs[4], c_scanRequestsDOWN, "C-SCAN DOWN", max_y)

        # Eliminar el subplot vacío sobrante
        fig.delaxes(axs[5])

        plt.tight_layout()
        plt.show()
    return 0

main()