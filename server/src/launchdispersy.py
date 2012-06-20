#!/usr/bin/python

"""
Logger format:
- Timestamp
- DISP_SYNC_IN
- Community ID
- Member ID
- LAN host
- LAN port
- WAN host
- WAN port
- Connection type
- Advice
- Sync time low (0 when sync disabled)
- Sync time high (0 when sync disabled)
- Sync modulo (1 when sync disabled)
- Sync offset (0 when sync disabled)
- Bytes transferred in response
"""

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

def main():
    sscfg = SessionStartupConfig()
    sscfg.set_state_dir(os.path.realpath("/tmp"))
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
    dispersy = Dispersy.has_instance()
    
    NetworkBuzzDBHandler.getInstance()
    
    def dispersyThread():
        while True:
            
            def printResultsFromDispersy(keywords, results, candidate):
                results_length = len(results)
                print >> sys.stderr,"TorrentSearchGridManager: gotRemoteHist: got", results_length,"unfiltered results for", keywords, candidate
                
                if results_length > 0:
                    finger = 0
                    for result in results:
                        print >> sys.stderr, "Result ", finger, ":", result[1]
                        swifthash = result[9]
                        
                        if swifthash:
                            if not isinstance(swifthash, str):
                                print >> sys.stderr, "Type error!"
                            else:
                                if len(swifthash) != 20:
                                    print >> sys.stderr, "Invalid swift hash!"
                                else:
                                    print >> sys.stderr, "swifthash = ", binascii.hexlify(swifthash)
                        
                        print >> sys.stderr, "========================================"
                        finger += 1
                
            for community in dispersy.get_communities():
                if isinstance(community, SearchCommunity):
                    nr_requests_made = community.create_search([u"Iron Man"], printResultsFromDispersy)
                    break
                    
            yield 3.0
    
    dispersy.callback.register(dispersyThread)

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
