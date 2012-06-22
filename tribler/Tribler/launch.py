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
import optparse
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
    command_line_parser = optparse.OptionParser()
    command_line_parser.add_option("--statedir", action="store", type="string", help="Use an alternate statedir")
    command_line_parser.add_option("--port", action="store", type="int", help="Listen at this port")
    command_line_parser.add_option("--dispersy-port", action="store", type="int", help="Dispersy uses this UDL port", default=6421)
    command_line_parser.add_option("--nickname", action="store", type="string", help="The moderator name", default="Booster")

    # parse command-line arguments
    opt, args = command_line_parser.parse_args()

    if not opt.statedir:
        command_line_parser.print_help()
        print "\nExample: python", sys.argv[0], "--statedir /home/tribler/booster --nickname Booster"
        sys.exit()

    print "Press Ctrl-C to stop the booster"

    sscfg = SessionStartupConfig()
    if opt.statedir: sscfg.set_state_dir(os.path.realpath(opt.statedir))
    if opt.port: sscfg.set_listen_port(opt.port)
    if opt.dispersy_port: sscfg.set_dispersy_port(opt.dispersy_port)
    if opt.nickname: sscfg.set_nickname(opt.nickname)
    
    sscfg.set_swift_proc(False)
    sscfg.set_dispersy(True)
    sscfg.set_megacache(True)
    sscfg.set_overlay(False)
    # turn torrent collecting on. this will cause torrents to be distributed
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
