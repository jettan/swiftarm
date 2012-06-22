#!/usr/bin/python

from traceback import print_exc
import os
import sys
import time
import binascii
from Tribler.Core.CacheDB.SqliteCacheDBHandler import NetworkBuzzDBHandler
from Tribler.Core.API import SessionStartupConfig, Session
from Tribler.Core.Statistics.Logger import OverlayLogger
from Tribler.dispersy.dispersy import Dispersy
from Tribler.dispersy.message import Message
from Tribler.community.search.community import SearchCommunity


search_community = []
dispersy = []

def dispersyDoSearch(keywords, callback):
    while True:
        if searchCommunity[0].get_nr_connections() < 5:
            yield 5.0
        else:
            print >> sys.stderr, "CREATING SEARCH RESULT!"
            nr_requests_made = searchCommunity[0].create_search(keywords, callback)
            print >> sys.stderr, nr_requests_made
            break

def search():
    print >> sys.stderr, "Going to search"
    dispersy[0].callback.register(dispersyDoSearch, args=([u"vodo"], printResultsFromDispersy))
    print >> sys.stderr, "Finished search"


def printResultsFromDispersy(keywords, results, candidate):
    results_length = len(results)
    print >> sys.stderr,"TorrentSearchGridManager: gotRemoteHist: got", results_length,"unfiltered results for", keywords, candidate

    if results_length > 0:
#        infohashes = [result[0] for result in results]
        
        finger = 0
        for result in results:
            print >> sys.stderr, "Result ", finger, ":", result[1]
            swifthash = result[9]
        
            if swifthash:
                if not isinstance(swifthash, str):
                    print >> sys.stderr, "Type error!"
                elif len(swifthash) != 20:
                    print >> sys.stderr, "Invalid swift hash!"
                else:
                    print >> sys.stderr, "swifthash = ", binascii.hexlify(swifthash)
            
            print >> sys.stderr, "========================================"
            finger += 1

def main():

#    while True:
#        print >> sys.stderr, "Going to loop!"
#        time.sleep(2)

    sscfg = SessionStartupConfig()
    sscfg.set_state_dir(unicode(os.path.realpath("/tmp")))
    sscfg.set_dispersy_port(6421)
    sscfg.set_nickname("dispersy")
    
    sscfg.set_swift_proc(False)
    sscfg.set_dispersy(True)
    sscfg.set_megacache(True)
    sscfg.set_overlay(False)
    sscfg.set_torrent_collecting(False)
    sscfg.set_dialback(False)
    sscfg.set_internal_tracker(False)

    session = Session(sscfg)
    time.sleep(5)
    dispersy.append(Dispersy.has_instance())
    
    NetworkBuzzDBHandler.getInstance()
    
#    def on_torrent(messages):
#        pass
    
    def findSearchCommunity():
        for community in dispersy[0].get_communities():
            if isinstance(community, SearchCommunity):
                 search_community.append(community)
#                 searchCommunity.on_torrent = on_torrent
                 break
    
    dispersy[0].callback.register(findSearchCommunity)
    
    
    # KeyboardInterrupt
    try:
        while True:
            sys.stdin.read()
    except:
        print_exc()

    session.shutdown()
    print "Shutting down..."
    time.sleep(5)

if __name__ == "__main__":
    main()
