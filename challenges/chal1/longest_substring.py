#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jan 15 20:29:53 2019

@author: oscar
"""

def longest_substring(string):
    visited = {}
    longest_string = ""
    
    last_known_repeated = 0
    
    for i in range(len(string)):    
        if string[i] not in visited:
            visited[string[i]] = i
            
            contestant_longest_string = i - last_known_repeated + 1
            
            if contestant_longest_string > len(longest_string):
                longest_string = string[last_known_repeated:i+1]
                #print("1:", longest_string)
            
        else:
            contestant_longest_string = i - visited[string[i]]
            
            if contestant_longest_string > len(longest_string):
                longest_string = string[visited[string[i]]:i]
                print("2: ", longest_string)
            
            last_known_repeated = visited[string[i]]+1
            
            visited[string[i]] = i
    
    if longest_string == "":
        longest_string = string
            
    return len(longest_string)
    
        

print(longest_substring("