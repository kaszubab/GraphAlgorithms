#!/usr/bin/env python
# coding: utf-8

# In[25]:


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


# In[26]:


def buildResidualNetwork(flowNetwork):
    residualNetwork = []
    for i in range(len(flowNetwork)) :
        residualNetwork.append([])
        for x in flowNetwork[i]:
            residualNetwork[i].append((x[0],x[2]))
    for i in range(len(flowNetwork)) :
        for x in flowNetwork[i]:
            residualNetwork[x[0]].append((i,x[1]))
    return residualNetwork


# In[27]:


def bfs(graph,s,t,parentTab):
    visited = [False for x in range(len(graph))]
    queue = []
    queue.append(s)
    while (queue):
        vertex = queue.pop(0)
        for x in graph[vertex]:
            if not visited[x[0]] and x[1] > 0:
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
    


# In[28]:


def fordFullkerson(name):
    flowNetwork = loadGraph(name)
    residualNetwork = buildResidualNetwork(flowNetwork)
    parentTab = [-1 for x in range(len(residualNetwork))]
    maxflow = 0
    while bfs(residualNetwork,0,len(residualNetwork)-1,parentTab) :
        flowSurplus = float("Inf")
        x = len(residualNetwork)-1
        while not x == 0 :
            currFlow = 0
            for y in residualNetwork[parentTab[x]] :
                if y[0] == x:
                    currFlow = y[1]
            flowSurplus = min(flowSurplus,currFlow)
            x = parentTab[x]
        maxflow += flowSurplus
        x = len(residualNetwork)-1
        while not x == 0 :
            z = residualNetwork[parentTab[x]]
            for y in range(len(z)) :
                if z[y][0] == x:
                    newFlow = z[y][1] - flowSurplus
                    z[y] = (z[y][0],newFlow)
            z = residualNetwork[x]
            for y in range(len(z)) :
                if z[y][0] == parentTab[x]:
                    z[y] = (z[y][0],z[y][1] + flowSurplus)
            x = parentTab[x]
    return maxflow
    
    
#%%


# In[32]:


file_name = "flow/rand100_500"   
print(fordFullkerson(file_name))
print(open(file_name).readline().split()[3])
#graph = loadGraph("simple")
#print (buildResidualNetwork(graph))


# In[ ]:




