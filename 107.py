import functools
from heapq import *

@functools.total_ordering
class Edge:
    def __init__(self, vA, vB, cost):
        self.vA = vA
        self.vB = vB
        self.cost = cost

    def __lt__(self, other):
        return self.cost < other.cost

    def __repr__(self):
        return "(vA=%d, vB=%d, cost=%d)" % (self.vA, self.vB, self.cost)

# read in the matrix and transform all "-" (i.e., no connection) to 0's
with open("network.txt") as f:
    matrix = [[int(n) if n != "-" else 0 for n in line.strip().split(",")] for line in f.readlines()]

# compile a list of adjacency lists, one for each vertex
adjlists = [list(filter(lambda e: e.cost > 0, (Edge(r, c, element) for c,element in enumerate(row)))) for r,row in enumerate(matrix)]

commit = [[] for i in range(len(matrix))]
verts = set()

queue = adjlists[0]
verts.add(queue[0].vA) # add the first node to be expanded
heapify(queue)

# Prim's algorithm
while len(verts) < len(matrix) and len(queue) > 0:
    e = heappop(queue)
    
    if e.vB not in verts:
        verts.add(e.vB)
        commit[e.vA].append(e)
        for x in adjlists[e.vB]:
            heappush(queue, x)

orig_cost = sum(sum(row[:r]) for r,row in enumerate(matrix))
new_cost = sum(map(lambda e: e.cost, sum(commit,[])))

print("Original Cost=%d; Reduced Cost=%d; Difference=%d" % (orig_cost, new_cost, orig_cost-new_cost))
