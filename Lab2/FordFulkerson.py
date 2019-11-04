# -*- coding: utf-8 -*-
"""
Created on Tue Oct 22 13:06:33 2019

@author: Admin
"""
import dimacs

def loadGraph(name):
    graph = dimacs.loadDirectedWeightedGraph(name)
    myGraph = [[] for x in range(graph[0])]
    for  x in graph[1] :
        myGraph[x[0]-1].append([x[1]-1,0,x[2]])
    return myGraph
#%%

def buildResidualNetwork(flowNetwork):
    #import copy
    #residualNetwork = copy.deepcopy(flowNetwork)
    residualNetwork = []
    for i in range(len(flowNetwork)) :
        residualNetwork.append([])
        for x in flowNetwork[i]:
            residualNetwork[i].append((x[0],x[2]))
    #print (residualNetwork)
    for i in range(len(flowNetwork)) :
        for x in flowNetwork[i]:
            #print (x)
            residualNetwork[x[0]].append((i,x[1]))
    return residualNetwork

def bfs(graph,s,t,parentTab):
    visited = [False for x in range(len(graph))]
    queue = []
    queue.append(s)
    while (queue):
        vertex = queue.pop(0)
        for x in graph[vertex]:
            if not visited[x[0]] and x[1]>0
                queue.append(x[0])
                visited[x[0]]=True
                parentTab[x[0]]=vertex
    return visited[t]

def dfs(graph,s,t,parentTab):
    visited = [False for x in range(len(graph))]
    stack = []
    stack.append(s)
    while(stack):
        vertex = stack.pop()
        for x in graph[vertex]:
            if not (visited[x[0]]) and x[1] > 0:
                stack.append(x[0])
                visited[x[0]]=True
                parentTab[x[0]]=vertex
    return (visited[t])
    


def fordFullkerson(name):
    flowNetwork = loadGraph(name)
    residualNetwork = buildResidualNetwork(flowNetwork)
    parentTab = [-1 for x in range(len(residualNetwork))]
    maxflow = 0
    #print (residualNetwork)
    while bfs(residualNetwork,0,len(residualNetwork)-1,parentTab) :
        #print ("dfs sie nie zapetla")
        flowSurplus = float("Inf")
        x = len(residualNetwork)-1
        while not x == 0 :
            currFlow = 0
            for y in residualNetwork[parentTab[x]] :
                if y[0] == x:
                    currFlow = y[1]
            flowSurplus = min(flowSurplus,currFlow)
            x = parentTab[x]
        #print (flowSurplus)
        maxflow += flowSurplus
        x = len(residualNetwork)-1
        #print (residualNetwork)
        while not x == 0 :
            z = residualNetwork[parentTab[x]]
            #print (z)
            for y in range(len(z)) :
                if z[y][0] == x:
                    newFlow = z[y][1] - flowSurplus
                        #print (newFlow)
                    z[y] = (z[y][0],newFlow)
            z = residualNetwork[x]
            for y in range(len(z)) :
                if z[y][0] == parentTab[x]:
                    z[y] = (z[y][0],z[y][1] + flowSurplus)
            x = parentTab[x]
        #print (residualNetwork)
        #print("ok")
    return maxflow
    
    
#%%
    
print(fordFullkerson("flow/grid100x100"))
#graph = loadGraph("simple")
#print (buildResidualNetwork(graph))
