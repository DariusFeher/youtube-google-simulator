#include "videoplayer.h"
#include <iostream>




void VideoPlayer::numberOfVideos() {
  std::cout << mVideoLibrary.getVideos().size() << " videos in the library"
            << std::endl;
}

void VideoPlayer::showAllVideos() {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  // sort the videos in ascending order by title
  std::sort(mVideos.begin(), mVideos.end(), [](const Video& v1, const Video& v2) {
      return v1.getTitle() < v2.getTitle();
   });
  std::cout << "Here's a list of all available videos:" << std::endl;
  // iterate over all videos and display their title, id and tags
  for (const auto& video : mVideos) {
    std::cout << "\t" << video.getTitle() << " (" << video.getVideoId() << ") [";
    std::vector<std::string> vTags = video.getTags();

    const int n = vTags.size();
    for (int i = 0; i < n - 1; ++i) {
      std::cout << vTags[i] << " ";
    }
    if (n >= 1) {
      std::cout << vTags[n - 1];
    }
    std::cout << "]";
    // Display if the video is flagged and its reason 
    if (flaggedVideos.count(video.getVideoId())) {
      std::cout << " - FLAGGED (reason: " << flaggedVideos[video.getVideoId()] << ")";
    }
    std::cout << std::endl; 
  }
}

void VideoPlayer::playVideo(const std::string& videoId) {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  // Check if videoId is a valid video id
  const auto found = find_if(mVideos.begin(), mVideos.end(), [videoId](const Video& video) {return video.getVideoId() == videoId;});
  if (found == mVideos.end()) {
    std::cout << "Cannot play video: Video does not exist" << std::endl;
  } else {
    if (flaggedVideos.count(videoId)) { // A flagged video cannot be played
      std::cout << "Cannot play video: Video is currently flagged (reason: " << flaggedVideos[videoId] << ")" << std::endl;
    } else {
      if (playingVideoId.empty()) { // No video is currently playing
        std::cout << "Playing video: " << found->getTitle() << std::endl;
        playingVideoId = videoId;
      } else {
        // Stop the current playing video and play the 'videoId' video.
        std::cout << "Stopping video: " <<  mVideoLibrary.getVideo(playingVideoId)->getTitle() << std::endl;
        std::cout << "Playing video: " << found->getTitle() << std::endl;
        playingVideoId = videoId;
      }
      videoStatus = "playing"; // Set the current video's status to playing
    }
  }

}

void VideoPlayer::stopVideo() {
  if (playingVideoId.empty()) {
    std::cout << "Cannot stop video: No video is currently playing" << std::endl;
  } else {
    std::cout << "Stopping video: " << mVideoLibrary.getVideo(playingVideoId)->getTitle() << std::endl;
    playingVideoId = "";
    videoStatus = "";
  }
}

void VideoPlayer::playRandomVideo() {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  std::vector<Video> notFlaggedVideos;

  for (Video video : mVideos) {
    // If the current video is not flagged, then add it to the notFlaggedVideos vector
    if (flaggedVideos.count(video.getVideoId()) == 0) {
      notFlaggedVideos.push_back(video);
    }
  }
  // If there is a video playing and there is at least one video which is not flagged, then stop the current video
  if (!playingVideoId.empty() && notFlaggedVideos.size()) {
    std::cout << "Stopping video: " << mVideoLibrary.getVideo(playingVideoId)->getTitle() << std::endl;
  }
  if (notFlaggedVideos.size()) { // If there are videos available which are not flagged
    int random = rand() % notFlaggedVideos.size();
    std::cout << "Playing video: " << notFlaggedVideos[random].getTitle() << std::endl;
    videoStatus = "playing";
    playingVideoId = notFlaggedVideos[random].getVideoId();
  } else {
    std::cout << "No videos available" << std::endl;
  }
}

void VideoPlayer::pauseVideo() {
  if (!playingVideoId.empty()) { // Check if there is a video playing
    if (videoStatus == "paused") { // Check if video is already paused
      std::cout << "Video already paused: " << mVideoLibrary.getVideo(playingVideoId)->getTitle() << std::endl;
    } else { // Video is playing and not paused
      std::cout << "Pausing video: " << mVideoLibrary.getVideo(playingVideoId)->getTitle() << std::endl;
      videoStatus = "paused";
    }
  } else { // No video is playing
    std::cout << "Cannot pause video: No video is currently playing" << std::endl; 
  }
}

void VideoPlayer::continueVideo() {
  if (playingVideoId.empty()) { // Check if there is a video playing
    std::cout << "Cannot continue video: No video is currently playing" << std::endl;
  } else {
    if (videoStatus != "paused") {
      std::cout << "Cannot continue video: Video is not paused" << std::endl;
    } else {
      std::cout << "Continuing video: Amazing Cats" << std::endl;
      videoStatus = "playing";  
    }
  }
}

void VideoPlayer::showPlaying() {
  if (playingVideoId.empty()) {
    std::cout << "No video is currently playing" << std::endl;
  } else {
    std::cout << "Currently playing: " << mVideoLibrary.getVideo(playingVideoId)->getTitle() << " (" << mVideoLibrary.getVideo(playingVideoId)->getVideoId() << ") [";
    std::vector<std::string> vTags = mVideoLibrary.getVideo(playingVideoId)->getTags();
    const int n = vTags.size();
    for (int i = 0; i < n - 1; ++i) {
      std::cout << vTags[i] << " ";
    }
    if (n >= 1) {
      std::cout << vTags[n - 1];
    }
    std::cout << "]";
    if (videoStatus == "paused") {
      std::cout << " - PAUSED";
    }
    std::cout << std::endl;
  }
}

void VideoPlayer::createPlaylist(const std::string& playlistName) {
  bool valid = true;
  // transform the playlistName to lower cases
  std::string playlistNameCopy = playlistName;
  std::transform(playlistNameCopy.begin(), playlistNameCopy.end(), playlistNameCopy.begin(), [](unsigned char c){ return std::tolower(c); });
  // iterate over the unordered map of playlists
  for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
    std::string key =  iter->first;
    // transofrm the key to lower cases
    std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
    if (key == playlistNameCopy) {
      valid = false; // There exist at least one playlist that has the same name, therefore the given name is not valid
      break;
    }
  }
  if (!valid) {
    std::cout << "Cannot create playlist: A playlist with the same name already exists" << std::endl;
  } else {
    std::vector<std::string> videos;
    VideoPlaylist vp = VideoPlaylist(videos);
    playlists.emplace(playlistName, vp);
    std::cout << "Successfully created new playlist: " << playlistName << std::endl;
  }
}

void VideoPlayer::addVideoToPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
  bool validPlaylist = false, validVideoId = true;
  // transform the playlistName to lower cases
  std::string playlistNameCopy = playlistName;
      std::transform(playlistNameCopy.begin(), playlistNameCopy.end(), playlistNameCopy.begin(), [](unsigned char c){ return std::tolower(c); });

  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  const auto found = find_if(mVideos.begin(), mVideos.end(), [videoId](const Video& video) {return video.getVideoId() == videoId;});
  if (found == mVideos.end()) {
    validVideoId = false;
  } 
  if (validVideoId && flaggedVideos.count(videoId)) {
      std::cout << "Cannot add video to " << playlistName << ": Video is currently flagged (reason: " << flaggedVideos[videoId] << ")" << std::endl;
  } else {
    for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
      std::string key =  iter->first;
      std::string keyCopy = key;

      // Convert to lower cases
      std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
      
      // Compare key and the given playlist name, both converted to lower case
      if (key == playlistNameCopy) {
        validPlaylist = true;
        // Iterate over videos in the playlist
        if (validVideoId) {
          VideoPlaylist vp = iter->second;
          int n = vp.getPlaylistVideos().size();
          bool videoInPlaylist = false;
          std::vector<std::string> playlistVideos = vp.getPlaylistVideos();

          for (int i = 0; i < n; ++i) {
            if (playlistVideos[i] == videoId) {
              videoInPlaylist = true;
            }
          }
          if (!videoInPlaylist) {
            playlistVideos.emplace_back(videoId);
            VideoPlaylist videoPlaylist = VideoPlaylist(playlistVideos);
            playlists.erase(keyCopy);
            playlists.emplace(keyCopy, videoPlaylist);
            std::cout << "Added video to " << playlistName << ": " << mVideoLibrary.getVideo(videoId)->getTitle() << std::endl;
          } else {
            std::cout << "Cannot add video to " << playlistName << ": Video already added" << std::endl;
          }
        }
        break;
      }
    }
    if (!validPlaylist) {
      std::cout << "Cannot add video to " << playlistName << ": Playlist does not exist" << std::endl;
    } else if (!validVideoId) {
      std::cout << "Cannot add video to " << playlistName << ": Video does not exist" << std::endl;
    }
  }
}

void VideoPlayer::showAllPlaylists() {
  if (playlists.size()) {
    std::cout << "Showing all playlists:" << std::endl;
    for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
      std::cout << "\t" << iter->first << std::endl;
    }
  } else {
    std::cout << "No playlists exist yet" << std::endl;
  }
}

void VideoPlayer::showPlaylist(const std::string& playlistName) {
  bool playlistValid = false;
 
  // Convert to lower cases
  std::string playlistNameCopy = playlistName;
  std::transform(playlistNameCopy.begin(), playlistNameCopy.end(), playlistNameCopy.begin(), [](unsigned char c){ return std::tolower(c); });
  // Iterate over the playlists unordered map
  for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
      std::string key =  iter->first;
      std::string keyCopy = key;

      // Convert to lower cases
      std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
      if (key == playlistNameCopy) {
        playlistValid = true;
        std::cout << "Showing playlist: " << playlistName << std::endl;
        VideoPlaylist vp = iter->second;
        int n = vp.getPlaylistVideos().size();
        std::vector<std::string> playlistVideos = vp.getPlaylistVideos();
        if (n) {
          for (std::string video : playlistVideos) {
            std::cout << "\t" << mVideoLibrary.getVideo(video)->getTitle() << " (" << mVideoLibrary.getVideo(video)->getVideoId() << ") [";
            std::vector<std::string> vTags = mVideoLibrary.getVideo(video)->getTags();
            const int n = vTags.size();
            for (int i = 0; i < n - 1; ++i) {
              std::cout << vTags[i] << " ";
            }
            if (n >= 1) {
              std::cout << vTags[n - 1];
            }
            std::cout << "]";
            if (flaggedVideos.count(video)) {
              std::cout << " - FLAGGED (reason: " << flaggedVideos[video] << ")";
            }
            std::cout << std::endl;
          }

        } else {
            std::cout << "No videos here yet" << std::endl;
        }
        break;
      }
    }
    if (!playlistValid) {
      std::cout << "Cannot show playlist " << playlistName << ": Playlist does not exist" << std::endl;
    }
}

void VideoPlayer::removeFromPlaylist(const std::string& playlistName,
                                     const std::string& videoId) {
  bool playlistValid = false;
  bool videoFound = false;

  // Convert to lower cases
  std::string playlistNameCopy = playlistName;
  std::transform(playlistNameCopy.begin(), playlistNameCopy.end(), playlistNameCopy.begin(), [](unsigned char c){ return std::tolower(c); });
  // Iterate over the playlists unordered map
  for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
      std::string key =  iter->first;
      std::string keyCopy = key;

      // Convert to lower cases
      std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
      if (key == playlistNameCopy) {
        playlistValid = true;
        VideoPlaylist vp = iter->second;
        std::vector<std::string> playlistVideos = vp.getPlaylistVideos();
        std::vector<std::string>::iterator it;
        it = playlistVideos.begin();
        for (auto it = playlistVideos.begin(); it != playlistVideos.end(); ++it) {
              if (it->c_str() == videoId) {
                playlistVideos.erase(it);
                videoFound = true;
                break;
              } 
        }
        if (videoFound) {
          playlists.erase(keyCopy);
          VideoPlaylist videoPlaylist = VideoPlaylist(playlistVideos);
          playlists.emplace(keyCopy, videoPlaylist);
          std::cout << "Removed video from " << playlistName << ": " << mVideoLibrary.getVideo(videoId)->getTitle() << std::endl;
        }
        break;
      }
    }
    bool validVideoId = true;
    std::vector<Video> mVideos =  mVideoLibrary.getVideos();
    const auto found = find_if(mVideos.begin(), mVideos.end(), [videoId](const Video& video) {return video.getVideoId() == videoId;});
    if (found == mVideos.end()) {
      validVideoId = false;
    }
    if (!playlistValid) {
      std::cout << "Cannot remove video from " << playlistName << ": Playlist does not exist" << std::endl;
    } else {
      if (validVideoId && !videoFound) {
        std::cout << "Cannot remove video from " << playlistName << ": Video is not in playlist" << std::endl;
      } else if (!validVideoId) {
        std::cout << "Cannot remove video from " << playlistName << ": Video does not exist" << std::endl;
      }
    }
}

void VideoPlayer::clearPlaylist(const std::string& playlistName) {
  bool playlistValid = false;
  // Convert to lower cases
  std::string playlistNameCopy = playlistName;
  std::transform(playlistNameCopy.begin(), playlistNameCopy.end(), playlistNameCopy.begin(), [](unsigned char c){ return std::tolower(c); });
  // Iterate over the playlists unordered map
  for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
      std::string key = iter->first;
      std::string keyCopy = key;

      // Convert to lower cases
      std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
      if (key == playlistNameCopy) {
        playlistValid = true;
        playlists.erase(keyCopy);
        std::vector<std::string> playlistVideos;
        VideoPlaylist videoPlaylist = VideoPlaylist(playlistVideos);
        playlists.emplace(keyCopy, videoPlaylist);
        std::cout << "Successfully removed all videos from " << playlistName << std::endl;
        break;
      }
    }
  if (!playlistValid) {
    std::cout << "Cannot clear playlist " << playlistName << ": Playlist does not exist" << std::endl;
  }
}

void VideoPlayer::deletePlaylist(const std::string& playlistName) {
  bool validPlaylist = false;
  // Convert to lower cases
  std::string playlistNameCopy = playlistName;
  std::transform(playlistNameCopy.begin(), playlistNameCopy.end(), playlistNameCopy.begin(), [](unsigned char c){ return std::tolower(c); });
  // Iterate over the playlists unordered map
  for(std::unordered_map<std::string, VideoPlaylist>::iterator iter = playlists.begin(); iter != playlists.end(); ++iter) {
      std::string key = iter->first;
      std::string keyCopy = key;

      // Convert to lower cases
      std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });
      if (key == playlistNameCopy) {
        validPlaylist = true;
        playlists.erase(keyCopy);
        std::cout << "Deleted playlist: " << playlistName << std::endl;
        break;
      }
    }

    if (!validPlaylist) {
      std::cout << "Cannot delete playlist " << playlistName << ": Playlist does not exist" << std::endl;
    }
}

bool isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void VideoPlayer::searchVideos(const std::string& searchTerm) {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  std::sort(mVideos.begin(), mVideos.end(), [](const Video& v1, const Video& v2) {
    return v1.getTitle() < v2.getTitle();
  });
  int nr = 0;
  std::vector<std::string> matchingVideos;
  for (const auto& video : mVideos) {
    if(flaggedVideos.count(video.getVideoId()) == 0) {
      std::string title = video.getTitle();
      std::transform(title.begin(), title.end(), title.begin(), [](unsigned char c){ return std::tolower(c); });
      if (title.find(searchTerm) != std::string::npos) {
        ++nr;
        if (nr == 1) {
          std::cout << "Here are the results for " << searchTerm << ":" << std::endl;
        }
        std::cout << "\t" << nr << ") ";
        std::cout << video.getTitle() << " (" << video.getVideoId() << ") [";
        std::vector<std::string> vTags = video.getTags();
        const int n = vTags.size();
        for (int i = 0; i < n - 1; ++i) {
          std::cout << vTags[i] << " ";
        }
        if (n >= 1) {
          std::cout << vTags[n - 1];
        }
        std::cout << "]" << std::endl;
        matchingVideos.push_back(video.getVideoId());
      }
    }
  }
  if (nr == 0) {
    std::cout << "No search results for " << searchTerm << std::endl;
  } else {
    std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << std::endl;
    std::cout << "If your answer is not a valid number, we will assume it's a no." << std::endl;
    std::string userInput;
    std::getline(std::cin, userInput); // Get user input
    if (isNumber(userInput)) { // Check if the input is a number
      int videoNr = stoi(userInput);
      if (videoNr <= nr) {
        playVideo(matchingVideos.at(videoNr - 1));
      }
    }
  }
}

void VideoPlayer::searchVideosWithTag(const std::string& videoTag) {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  std::sort(mVideos.begin(), mVideos.end(), [](const Video& v1, const Video& v2) {
    return v1.getTitle() < v2.getTitle();
  });
  int nr = 0;
  std::vector<std::string> matchingVideos;
  for (const auto& video : mVideos) {
    if(flaggedVideos.count(video.getVideoId()) == 0) {
      std::string tagCopy = videoTag;
      std::transform(tagCopy.begin(), tagCopy.end(), tagCopy.begin(), [](unsigned char c){ return std::tolower(c); });
      std::vector<std::string> vTags = video.getTags();
      const int n = vTags.size();
      for (int i = 0; i < n; ++i) {
        std::string tag = vTags[i];
        std::transform(tag.begin(), tag.end(), tag.begin(), [](unsigned char c){ return std::tolower(c); });
        if (tag == tagCopy) {
          ++nr;
          if (nr == 1) {
            std::cout << "Here are the results for " << videoTag << ":" << std::endl;
          }
          std::cout << "\t" << nr << ") ";
          std::cout << video.getTitle() << " (" << video.getVideoId() << ") [";
          std::vector<std::string> vTags = video.getTags();
          const int n = vTags.size();
          for (int i = 0; i < n - 1; ++i) {
            std::cout << vTags[i] << " ";
          }
          if (n >= 1) {
            std::cout << vTags[n - 1];
          }
          std::cout << "]" << std::endl;
          matchingVideos.push_back(video.getVideoId());
          break;
        }
      }
    }
  }
  if (nr == 0) {
    std::cout << "No search results for " << videoTag << std::endl;
  } else {
    std::cout << "Would you like to play any of the above? If yes, specify the number of the video." << std::endl;
    std::cout << "If your answer is not a valid number, we will assume it's a no." << std::endl;
    std::string userInput;
    std::getline(std::cin, userInput); // Get user input
    if (isNumber(userInput)) { // Check if the input is a number
      int videoNr = stoi(userInput);
      if (videoNr <= nr) {
        playVideo(matchingVideos.at(videoNr - 1));
      }
    }
  }
}

void VideoPlayer::flagVideo(const std::string& videoId) {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  const auto found = find_if(mVideos.begin(), mVideos.end(), [videoId](const Video& video) {return video.getVideoId() == videoId;});
  if (found == mVideos.end()) {
      std::cout << "Cannot flag video: Video does not exist" << std::endl;
  } else {
    if (flaggedVideos.count(videoId) == 0) {
      flaggedVideos.emplace(videoId, "Not supplied");
      if (playingVideoId == videoId) {
        stopVideo();
      }
      std::cout << "Successfully flagged video: " << mVideoLibrary.getVideo(videoId)->getTitle() << " (reason: Not supplied)" << std::endl;
    } else {
      std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    }
  }
}

void VideoPlayer::flagVideo(const std::string& videoId, const std::string& reason) {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  const auto found = find_if(mVideos.begin(), mVideos.end(), [videoId](const Video& video) {return video.getVideoId() == videoId;});
  if (found == mVideos.end()) {
      std::cout << "Cannot flag video: Video does not exist" << std::endl;
  } else {
    if (flaggedVideos.count(videoId) == 0) {
      flaggedVideos.emplace(videoId, reason);
      if (playingVideoId == videoId) {
        stopVideo();
      }
      std::cout << "Successfully flagged video: " << mVideoLibrary.getVideo(videoId)->getTitle() << " (reason: "<< reason << ")" << std::endl;
    } else {
      std::cout << "Cannot flag video: Video is already flagged" << std::endl;
    }
  }
}

void VideoPlayer::allowVideo(const std::string& videoId) {
  std::vector<Video> mVideos =  mVideoLibrary.getVideos();
  const auto found = find_if(mVideos.begin(), mVideos.end(), [videoId](const Video& video) {return video.getVideoId() == videoId;});
  if (found == mVideos.end()) {
    std::cout << "Cannot remove flag from video: Video does not exist" << std::endl;
  } else if (flaggedVideos.count(videoId) == 0){
    std::cout << "Cannot remove flag from video: Video is not flagged" << std::endl;
  } else {
    flaggedVideos.erase(videoId);
    std::cout << "Successfully removed flag from video: " << mVideoLibrary.getVideo(videoId)->getTitle() << std::endl;
  }
}

std::string VideoPlayer::getPlayingVideoId() {
  return playingVideoId;
}

std::string VideoPlayer::getPlayingVideoStatus() {
  return videoStatus;
}

std::unordered_map<std::string, VideoPlaylist> VideoPlayer::getPlaylists() {
  return playlists;
}

