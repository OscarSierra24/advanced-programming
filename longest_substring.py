#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Jan 15 20:29:53 2019

@author: oscar
"""

def longest_substring(string):
    visited = {}
    longest_string = ""
    
    for i in range(len(string)):        
        last_equal_char_known_pos = 0
        
        if string[i] not in visited:
            visited[string[i]] = i
        else:
            contestant_longest_string = i - visited[string[i]]
            
            if contestant_longest_string > len(longest_string):
                longest_string = string[visited[string[i]]:i]
            
            visited[string[i]] = i
    
    if longest_string == "":
        longest_string = string
            
    return longest_string
    
        

print(longest_substring("abcabcbb"))
