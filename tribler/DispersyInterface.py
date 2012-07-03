#!/usr/bin/python

from traceback import print_exc
import os
import sys
import time
import binascii
import threading

from Tribler.Core.CacheDB.SqliteCacheDBHandler import NetworkBuzzDBHandler
from Tribler.Core.API import SessionStartupConfig, Session
from Tribler.dispersy.dispersy import Dispersy
from Tribler.dispersy.message import Message
from Tribler.community.search.community import SearchCommunity


# Container for the search community of dispersy.
search_community = []

# Container for the dispersy instance.
dispersy = []

# List containing all search results to be sent back to C++.
search_results = []

# Mutex for locking the search results.
result_lock = threading.Lock()

# The actual search function done in the dispersy thread.
# MUST be done in the dispersy thread.
def dispersyDoSearch(keywords, callback):
    while True:
        if search_community[0].get_nr_connections() < 1:
            print >> sys.stderr, "Nobody connected in search community..."
            yield 5.0
        else:
            print >> sys.stderr, "Creating search request..."
            nr_requests_made = search_community[0].create_search(keywords, callback)
            print >> sys.stderr, nr_requests_made
            break

# Function to get the search results. Called from the C++ side.
def getSearchResults():
    result_lock.acquire()
    return_value = []
    try:
        return_value = search_results
    finally:
        result_lock.release()
        return return_value

# Function that lets the dispersy thread do the search.
# param search_term: The search term got from the C++ side.
def search(search_term):
    global search_results
    search_results = []
    print >> sys.stderr, "Searching for: ", search_term
    dispersy[0].callback.register(dispersyDoSearch, args=([unicode(search_term)], resultsCallback))
    print >> sys.stderr, "Finished search"

# Function called by dispersy when results come in.
def resultsCallback(keywords, results, candidate):
    results_length = len(results)
    print >> sys.stderr,"Got", results_length,"unfiltered results for", keywords, candidate

    if results_length > 0:
#        infohashes = [result[0] for result in results]
        result_lock.acquire()
        try:
            finger = 0
            for result in results:
                swifthash = result[8]
            
                if swifthash:
                    if not isinstance(swifthash, str):
                        print >> sys.stderr, "Type error!"
                    elif len(swifthash) != 20:
                        print >> sys.stderr, "Invalid swift hash!"
                    else:
                        search_result = binascii.hexlify(swifthash) + ":"+ result[1]
                        print >> sys.stderr, search_result
                        search_results.append(search_result)
                finger += 1
        finally:
            print >> sys.stderr, "-----------------------------------------------"
            result_lock.release()

# Main function to start a dispersy session and DHT in background.
def main():
    sscfg = SessionStartupConfig()
    sscfg.set_state_dir(unicode(os.path.realpath("/mtd_down/widgets/user/SamyGO/SamyGO/tribler/.tribler")))
    sscfg.set_dispersy_port(6421)
    sscfg.set_nickname("dispersy")
    
    # The only modules needed by dispersy and DHT.
    sscfg.set_dispersy(True)
    sscfg.set_megacache(True)
    
    # Disable all other tribler modules.
    sscfg.set_swift_proc(False)
    sscfg.set_buddycast(False)
    sscfg.set_social_networking(False)
    sscfg.set_remote_query(False)
    sscfg.set_bartercast(False)
    sscfg.set_overlay(False)
    sscfg.set_torrent_collecting(False)
    sscfg.set_dialback(False)
    sscfg.set_internal_tracker(False)
    
    # Create the session and wait for it to be created.
    session = Session(sscfg)
    time.sleep(5)
    
    # Create the dispersy instance and make it accessible out of the main().
    dispersy.append(Dispersy.has_instance())
    
    # Create the NetworkBuzzDBHandler that should be made in the tribler GUI.
    NetworkBuzzDBHandler.getInstance()
    
#    def on_torrent(messages):
#        pass
    
    # Find the search community from the dispersy instance.
    def findSearchCommunity():
        for community in dispersy[0].get_communities():
            if isinstance(community, SearchCommunity):
                 search_community.append(community)
#                 searchCommunity.on_torrent = on_torrent
                 break
    
    # Let the dispersy thread find the search community.
    # MUST be called on the dispersy thread.
    dispersy[0].callback.register(findSearchCommunity)
    
    # Any search request before this point will create a segfault!
    # TODO: Disable searching on C++ side before this happens.
    print >> sys.stderr, "Ready to search!"
    
    # Keep the main function spinning to keep the session alive and dispersy and DHT running.
    try:
        while True:
            sys.stdin.read()
    except:
        print_exc()
    
    # Shutdown everything.
    session.shutdown()
    print "Shutting down..."
    time.sleep(5)


if __name__ == "__main__":
    main()
