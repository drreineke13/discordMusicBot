//build command:
// g++ ./main.cpp ./dlHandler -ldpp -lopus -lopusfile -logg -I/usr/include/opus -o audioBot

#include "main.hpp"
#include "token.h"

//function to play audio from the bot
void playAudio(dpp::voiceconn* v, std::string songName, dlHandler dl){  
	            if (v && v->voiceclient && v->voiceclient->is_ready()) {
	                ogg_sync_state oy; 
	                ogg_stream_state os;
	                ogg_page og;
	                ogg_packet op;
	                OpusHead header;
	                char *buffer;
	 
	                FILE *fd;
	 
                    errno = 0;

	                fd = fopen(songName.c_str(), "r");

                    if(fd == NULL){
                        printf("Error %d \n", errno);
                        exit(1);
                    }

	                fseek(fd, 0L, SEEK_END);
	                size_t sz = ftell(fd);
	                rewind(fd);

	                ogg_sync_init(&oy);
	 
	                int eos = 0;
	                int i;
	 
	                buffer = ogg_sync_buffer(&oy, sz);
	                fread(buffer, 1, sz, fd);
	 
	                ogg_sync_wrote(&oy, sz);
	 
	                if (ogg_sync_pageout(&oy, &og) != 1)
	                {
	                    fprintf(stderr,"Does not appear to be ogg stream.\n");
	                    exit(1);
	                }
	 
                    
	                ogg_stream_init(&os, ogg_page_serialno(&og));
	 
	                if (ogg_stream_pagein(&os,&og) < 0) {
	                    fprintf(stderr,"Error reading initial page of ogg stream.\n");
	                    exit(1);
	                }
	 
	                if (ogg_stream_packetout(&os,&op) != 1)
	                {
	                    fprintf(stderr,"Error reading header packet of ogg stream.\n");
	                    exit(1);
	                }
	 
	                /* We must ensure that the ogg stream actually contains opus data */
	                if (!(op.bytes > 8 && !memcmp("OpusHead", op.packet, 8)))
	                {
	                    fprintf(stderr,"Not an ogg opus stream.\n");
	                    exit(1);
	                }
	 
	                /* Parse the header to get stream info */
	                int err = opus_head_parse(&header, op.packet, op.bytes);
	                if (err)
	                {
	                    fprintf(stderr,"Not a ogg opus stream\n");
	                    exit(1);
	                }
	                /* Now we ensure the encoding is correct for Discord */
	                if (header.channel_count != 2 && header.input_sample_rate != 48000)
	                {
	                    fprintf(stderr,"Wrong encoding for Discord, must be 48000Hz sample rate with 2 channels.\n");
	                    exit(1);
	                }

                    

	                /* Now loop though all the pages and send the packets to the vc */
	                while (ogg_sync_pageout(&oy, &og) == 1){
	                    ogg_stream_init(&os, ogg_page_serialno(&og));
	 
	                    if(ogg_stream_pagein(&os,&og)<0){
	                        fprintf(stderr,"Error reading page of Ogg bitstream data.\n");
	                        exit(1);
	                    }

	                    while (ogg_stream_packetout(&os,&op) != 0)
	                    {
	                        /* Read remaining headers */
	                        if (op.bytes > 8 && !memcmp("OpusHead", op.packet, 8))
	                        {
	                            int err = opus_head_parse(&header, op.packet, op.bytes);
	                            if (err)
	                            {
	                                fprintf(stderr,"Not a ogg opus stream\n");
	                                exit(1);
	                            }
	                            if (header.channel_count != 2 && header.input_sample_rate != 48000)
	                            {
	                                fprintf(stderr,"Wrong encoding for Discord, must be 48000Hz sample rate with 2 channels.\n");
	                                exit(1);
	                            }
	                            continue;
	                        }
	                        /* Skip the opus tags */
	                        if (op.bytes > 8 && !memcmp("OpusTags", op.packet, 8))
	                            continue; 
	 
	                        /* Send the audio */
	                        int samples = opus_packet_get_samples_per_frame(op.packet, 48000);
	 
	                        v->voiceclient->send_audio_opus(op.packet, op.bytes, samples / 48);
	                    }
	                }
	 
	                /* Cleanup */
	                ogg_stream_clear(&os);
	                ogg_sync_clear(&oy);
	            }

}


//function to play audio from the bot
void playAudioLoop(dpp::discord_voice_client* v, std::string songName, dlHandler dl){  
	            if (v&& v->is_ready()) {
	                ogg_sync_state oy; 
	                ogg_stream_state os;
	                ogg_page og;
	                ogg_packet op;
	                OpusHead header;
	                char *buffer;
	 
	                FILE *fd;
	 
                    errno = 0;

	                fd = fopen(songName.c_str(), "r");

                    if(fd == NULL){
                        printf("Error %d \n", errno);
                        exit(1);
                    }

	                fseek(fd, 0L, SEEK_END);
	                size_t sz = ftell(fd);
	                rewind(fd);

	                ogg_sync_init(&oy);
	 
	                int eos = 0;
	                int i;
	 
	                buffer = ogg_sync_buffer(&oy, sz);
	                fread(buffer, 1, sz, fd);
	 
	                ogg_sync_wrote(&oy, sz);
	 
	                if (ogg_sync_pageout(&oy, &og) != 1)
	                {
	                    fprintf(stderr,"Does not appear to be ogg stream.\n");
	                    exit(1);
	                }
	 
                    
	                ogg_stream_init(&os, ogg_page_serialno(&og));
	 
	                if (ogg_stream_pagein(&os,&og) < 0) {
	                    fprintf(stderr,"Error reading initial page of ogg stream.\n");
	                    exit(1);
	                }
	 
	                if (ogg_stream_packetout(&os,&op) != 1)
	                {
	                    fprintf(stderr,"Error reading header packet of ogg stream.\n");
	                    exit(1);
	                }
	 
	                /* We must ensure that the ogg stream actually contains opus data */
	                if (!(op.bytes > 8 && !memcmp("OpusHead", op.packet, 8)))
	                {
	                    fprintf(stderr,"Not an ogg opus stream.\n");
	                    exit(1);
	                }
	 
	                /* Parse the header to get stream info */
	                int err = opus_head_parse(&header, op.packet, op.bytes);
	                if (err)
	                {
	                    fprintf(stderr,"Not a ogg opus stream\n");
	                    exit(1);
	                }
	                /* Now we ensure the encoding is correct for Discord */
	                if (header.channel_count != 2 && header.input_sample_rate != 48000)
	                {
	                    fprintf(stderr,"Wrong encoding for Discord, must be 48000Hz sample rate with 2 channels.\n");
	                    exit(1);
	                }

                    

	                /* Now loop though all the pages and send the packets to the vc */
	                while (ogg_sync_pageout(&oy, &og) == 1){
	                    ogg_stream_init(&os, ogg_page_serialno(&og));
	 
	                    if(ogg_stream_pagein(&os,&og)<0){
	                        fprintf(stderr,"Error reading page of Ogg bitstream data.\n");
	                        exit(1);
	                    }

	                    while (ogg_stream_packetout(&os,&op) != 0)
	                    {
	                        /* Read remaining headers */
	                        if (op.bytes > 8 && !memcmp("OpusHead", op.packet, 8))
	                        {
	                            int err = opus_head_parse(&header, op.packet, op.bytes);
	                            if (err)
	                            {
	                                fprintf(stderr,"Not a ogg opus stream\n");
	                                exit(1);
	                            }
	                            if (header.channel_count != 2 && header.input_sample_rate != 48000)
	                            {
	                                fprintf(stderr,"Wrong encoding for Discord, must be 48000Hz sample rate with 2 channels.\n");
	                                exit(1);
	                            }
	                            continue;
	                        }
	                        /* Skip the opus tags */
	                        if (op.bytes > 8 && !memcmp("OpusTags", op.packet, 8))
	                            continue; 
	 
	                        /* Send the audio */
	                        int samples = opus_packet_get_samples_per_frame(op.packet, 48000);
	 
	                        v->send_audio_opus(op.packet, op.bytes, samples / 48);
	                    }
	                }
	 
	                /* Cleanup */
	                ogg_stream_clear(&os);
	                ogg_sync_clear(&oy);
	            }

}

int main(int argc, char const *argv[]){
    dpp::cluster bot(TOKEN, dpp::i_default_intents | dpp::i_message_content);
	//class that handles all the download and queue functionality of the bot
	dlHandler downloader = dlHandler();

	//starts by making sure the are no files that should be overwritten
	downloader.clear();

    bot.on_log(dpp::utility::cout_logger());

	//handler for slash commands in disc
    bot.on_slashcommand([&downloader, &bot](const dpp::slashcommand_t& event){
		//default test command for call and response
        if (event.command.get_command_name() == "ping"){
            event.reply("Pong!");
        }
		//command to join the user's current audio channel
        if(event.command.get_command_name() == "join"){
            dpp::guild *g = dpp::find_guild(event.command.guild_id);
            //event.command.msg.author.id 
            if(!g->connect_member_voice(event.command.usr.id)){
                event.reply("Yowre nwot in a vowice chawannel ~-~");
                std::cout << event.command.msg.author.id << std::endl;
            }
            else{
                event.reply("Co-Connewcted to teh vowice chawannel ^w^");
            }
        }
		//command to stop playing a leave the audio channel
		//also clears the queue
        if(event.command.get_command_name() == "stop"){
            event.from->disconnect_voice(event.command.guild_id);
			downloader.clear();
			downloader.setStopped();
			event.reply("Lewft teh vowice chawannel ~w~");
        }
		//command to begin playing the songs currently in the queue
		if(event.command.get_command_name() == "play"){

			dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);

			std::string songName = downloader.getNext();
			event.reply("Now plawying moosic ^w^");
			downloader.setPlaying();
			playAudio(v, songName, downloader);
		}
		//clears the current queue
		if(event.command.get_command_name() == "clear"){
			downloader.clear();
			event.reply("I has cleawered teh q >w<");
		}
		//adds the url to the queue
		if(event.command.get_command_name() == "add"){

			std::string url = std::get<std::string>(event.get_parameter("url"));
			std::string outText = "Attwemptings to puwt swong in q awt nowomber ";
			int prevSize = downloader.size();
			outText.append(std::to_string(prevSize + 1));
			outText.append(" OvO");
			event.reply(outText);
			int i = downloader.downloadURL(url);
			if(prevSize + 1 != downloader.size()){
				bot.message_create(dpp::message(event.command.channel_id,"F-fawild to add swong... ~w~"));
			}

			downloader.setPlaying();

			dpp::guild *g = dpp::find_guild(event.command.guild_id);

			if(!g->connect_member_voice(event.command.usr.id)){
                bot.message_create(dpp::message("Yowre nwot in a vowice chawannel ~-~"));
            }

		}

		if(event.command.get_command_name() == "skip"){
			dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
			v->voiceclient->stop_audio();
			event.reply("Sk-Skwipping teh swong Owo");
			if(downloader.playing()){
				std::string songName = downloader.getNext();
				playAudio(v, songName, downloader);
				if(!downloader.hasNext()){
					downloader.setStopped();
				}
			}
		}
    });

	if(argc > 1 && (std::string(argv[1]) == "-u" || std::string(argv[1]) == "--update")){
		std::cout << "attempting to update commands" << std::endl;
		bot.on_ready([&bot, &downloader](const dpp::ready_t& event){
			bot.global_command_create(dpp::slashcommand("ping", "ping pong", bot.me.id));
            bot.global_command_create(dpp::slashcommand("join","join current voice channel",bot.me.id));
            bot.global_command_create(dpp::slashcommand("play", "play audio", bot.me.id));
            bot.global_command_create(dpp::slashcommand("stop", "stops bot and leaves vc", bot.me.id));
			bot.global_command_create(dpp::slashcommand("clear", "clears the current song queue", bot.me.id));
			dpp::slashcommand addCmd("add","adds url to queue",bot.me.id);
			addCmd.add_option(dpp::command_option(dpp::co_string, "url", "YouTube url", true));
			bot.global_command_create(addCmd);
			bot.global_command_create(dpp::slashcommand("skip","skips the current song",bot.me.id));
		});
	}

	bot.on_voice_ready([&downloader](const dpp::voice_ready_t& event){

		if(!event.voice_client->is_playing() && downloader.playing()){
			std::cout<< "ready to play" << std::endl;
		}

	});

	bot.on_voice_buffer_send([&bot, &downloader](const dpp::voice_buffer_send_t& event){
		if(!event.voice_client->is_playing() && downloader.playing()){
			std::string songName = downloader.getNext();
			playAudioLoop(event.voice_client, songName, downloader);
			if(!downloader.hasNext()){
				downloader.setStopped();
			}
		}

	});

	//standard bot commands for disc
    bot.on_message_create([&bot, &downloader](const dpp::message_create_t& event){
        std::stringstream ss(event.msg.content);
        std::string cmd;
        ss >> cmd;

        std::cout << cmd << std::endl;

		//joins the audio channel
        if(cmd == "sbjoin"){
            dpp::guild *g = dpp::find_guild(event.msg.guild_id);
            if(!g->connect_member_voice(event.msg.author.id)){
                bot.message_create(dpp::message(event.msg.channel_id,"Yowre nwot in a vowice chawannel ~-~"));
            }
        }

		//stops playing and leaves the channel
		//also clears the queue
        if(cmd == "sbstop"){
            event.from->disconnect_voice(event.msg.guild_id);
			downloader.clear();
			bot.message_create(dpp::message(event.msg.channel_id,"Lewft teh vowice chawannel ~w~"));
        }

		//plays the currently queued audio files
        if (cmd == "sbplay") {
			std::string songName = downloader.getNext();
			dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
	        playAudio(v, songName, downloader);
			bot.message_create(dpp::message(event.msg.channel_id, "now plawying moosic"));
		}

		//downloads a given url
		//need to finish the implementation of url parsing
		if(cmd == "sbdl"){
			std::string url;
			ss >> url;
			int i = downloader.downloadURL(url);
			std::string outText = "Song added to queue and is number ";
			outText.append(std::to_string(i));
			bot.message_create(dpp::message(event.msg.channel_id, outText));
		}

		//clears the queue
		if(cmd == "sbclear"){
			downloader.clear();
			bot.message_create(dpp::message(event.msg.channel_id, "I has cleawered teh q >w<"));
		}

	    });

    bot.start(dpp::st_wait);
    return 0;
}
