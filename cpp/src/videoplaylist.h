#pragma once

/** A class used to represent a Playlist */
#include <vector>
#include <string>

class VideoPlaylist {
	private:
		std::vector<std::string> playlistVideos;
	public:
		VideoPlaylist(std::vector<std::string>& videos);

		// Returns a list containing the ids of all the videos from a specific playlist.
  		const std::vector<std::string>& getPlaylistVideos() const;
};
