#include "videoplaylist.h"

#include <iostream>
#include <utility>
#include <vector>


VideoPlaylist::VideoPlaylist(std::vector<std::string>& videos) :
  playlistVideos(std::move(videos)){
}

const std::vector<std::string>& VideoPlaylist::getPlaylistVideos() const { return playlistVideos; }
