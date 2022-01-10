import heapq
from PIL import Image, ImageDraw

graph = [[1],[0,3],[3,4],[1,2,9],[2,5],[4,6,16],[5,7],[6,8],[7],[3,10],[9,11],[10,12],
        [11,13],[12,14,21],[13,15],[14],[5,17],[16,18],[17,19],[18,20],[19,22],[13,22],[20,21,23],[22]]
dst = [0,8,15]
device = [1,3,5,7,13,20,23]
edges = [ [ 1 for i in range(25) ] for j in range(25) ]

def bfs(start):
    visited = []
    q = []
    path = []
    heapq.heappush(q,(0,(start,[start])))
    while q:
        val, (now,route) = heapq.heappop(q)
        if now in dst:
            dist = val
            path = route
            break
        visited.append(now)
        neighbors = graph[now]
        for neighbor in neighbors:
            if neighbor not in visited:
                if edges[now][neighbor] == -1:
                    visited.append(neighbor)
                    continue
                heapq.heappush(q,(val+edges[now][neighbor],(neighbor,route+[neighbor])))
    return path, val

def setFirePoint(fires):
    for fire in fires:
        for node in graph[fire]:
            edges[node][fire] = -1
            edges[fire][node] = -1


print('Indexes of Devices: ', device)
print('Enter Fire positions: ')
arr = input()   # takes the whole line of n numbers
firePos = list(map(int,arr.split(' ')))

setFirePoint(firePos) 

coordinates = [(170,133),(170,169),(170,288),(170,256),(170,480),(170,553),
                (170,670),(170,690),(170,710),(258,256),(360,256),(463,256),
                (575,256),(650,256),(820,256),(840,256),(258,553),(360,553),
                (450,553),(555,553),(590,553),(650,333),(650,553),(650,660)]


for node in device:
    im = Image.open("clean_map.jpg")
    path, val = bfs(node)
    im_drawed = im
    draw = ImageDraw.Draw(im_drawed)

    draw.ellipse((coordinates[node][0]-10, coordinates[node][1]-10, coordinates[node][0]+10, coordinates[node][1]+10), fill = 'red', outline ='red')

    if path == []:
        im_drawed.save("static/imgs/%s.jpg"%(str(node)))
        continue

    for idx, _ in enumerate(path):
        if idx == len(path)-1:
            continue
        draw.line((coordinates[path[idx]][0],coordinates[path[idx]][1], coordinates[path[idx+1]][0], coordinates[path[idx+1]][1]), fill=255, width=3)

    im_drawed.save("static/imgs/%s.jpg"%(str(node)))
