
// C++
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
// ROS msg
#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/String.h>
// voztts
#include "voztts/cached_files_map.h"


enum Engine {
  ENGINE_MARYTTS,
};
std::string _scripts_folder = ros::package::getPath("voztts") + "/scripts";
std::string effects ;
std::string pitch;
std::string range;
std::string rate;
std::string volume;
std::string contour;
std::string emotion_name;
Engine _engine;
utils::Language _language = "en";
std::vector<std::string> _versions;
// ENGINE_MARYTTS
static const std::string TMP_MARYTTS_FILE = "/tmp/voztts_marytts.wav";
CachedFilesMap _marytts_cache(ros::package::getPath("voztts") + "/data/marytts_cache/index.csv");

bool mplayer_file(const std::string & filename)  {
  //ROS_INFO("mplayer_file(%s)", filename.c_str());
  std::ostringstream play_cmd;
  // read it with mplayer
  play_cmd.str("");
  play_cmd << "mplayer " << filename << " -really-quiet ";
  play_cmd << " 2> /dev/null ";
  ROS_INFO("running command '%s'", play_cmd.str().c_str());
  return (utils::exec_system(play_cmd.str().c_str()) == 0);
}


//! \return true if file cached and could be played
bool mplayer_cached_file_if_available(CachedFilesMap & cache,
                                      const std::string & key) {
  std::string cached_file;
  if (!cache.get_cached_file(key, cached_file)) {
        ROS_INFO("sentence '%s' was not in cache:'%s'",
               key.c_str(), cached_file.c_str());
    return false;
  }
  ROS_INFO("sentence '%s' was already in cache:'%s'",
           key.c_str(), cached_file.c_str());
  return mplayer_file(cached_file);
}

void tts_cb(const std_msgs::StringConstPtr & msg) {
  // split sentence and find correct language
  utils::stringSplit(msg->data, "|", &_versions);
  std::string tosay = "";
  if (!utils::find_given_language_in_multilanguage_line(_versions, _language, tosay))
    tosay = msg->data;
  std::string key = _language + ":" + tosay;
  // clean sentence
  std::string tosay_clean = tosay;
  utils::find_and_replace(tosay_clean, "\"", "\\\"");
  // generate command
  std::ostringstream command;
  
  
  if (_engine == ENGINE_MARYTTS) {
    // list languages: http://localhost:59125/locales: de en_GB en_US fr it lb ru sv te tr
    //if (mplayer_cached_file_if_available(_marytts_cache, key))
    //  return;
    command << "wget ";
    command << "\"http://localhost:59125/process?INPUT_TYPE=RAWMARYXML";
    command << "&OUTPUT_TYPE=AUDIO";
    command << "&AUDIO_OUT=WAVE_FILE";
    command << "&AUDIO=WAVE_FILE";

    if (_language == "en")
      command << "&LOCALE=en-US";
    else
      command << "&LOCALE=" << _language;
    if (_language == "en")
      command << "&VOICE=cmu-slt-hsmm"; // en_US female hmm
    else if (_language == "fr")
      command << "&VOICE=upmc-pierre-hsmm"; // fr male hmm
    else {
      ROS_WARN("TESTE language'%s'", _language.c_str());
      command << "&VOICE=cmu-slt-hsmm"; // en_US female hmm
    }
    //command << ("%s",effects.c_str());
    //command << "&INPUT_TEXT=" << "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n<maryxml version=\'0.4\'\nxmlns:xsi=\'http://www.w3.org/2001/XMLSchema-instance\'\nxmlns=\'http://mary.dfki.de/2002/MaryXML\'\nxml:lang=\'en-US\'>\n\n<prosody pitch=\'+50%\' range=\'200%\' rate=\'90%\' volume=\'80%\' contour=\'(10%,+80%)\' >\n i am so happy Dieisson.\n</prosody>\n</maryxml>\n";
    
    command << "&INPUT_TEXT=" << "<?xml version=\'1.0\' encoding=\'UTF-8\'?>\n<maryxml version=\'0.4\'\nxmlns:xsi=\'http://www.w3.org/2001/XMLSchema-instance\'\nxmlns=\'http://mary.dfki.de/2002/MaryXML\'\nxml:lang=\'en-US\'>\n\n<prosody pitch=\'"+ pitch +"\' range=\'"+ range +"\' rate=\'"+ rate+"\' volume=\'"+ volume +"\' contour=\'"+ contour +"\' >\n" + tosay_clean + "\n</prosody>\n</maryxml>\n";

    command << "\"";
    command << " --output-document=" << TMP_MARYTTS_FILE;
   
    
    if (utils::exec_system((command.str().c_str())) == 0
        && mplayer_file(TMP_MARYTTS_FILE))
      _marytts_cache.add_cached_file(key, TMP_MARYTTS_FILE);
  } // end if (ENGINE_ENGINE_MARYTTS)
 
} // end tts_cb()
void callback(const std_msgs::String::ConstPtr& msg)
{
 
    
    ROS_WARN("Emotions changed to [%s]", msg->data.c_str());
    emotion_name = msg->data.c_str();
   //inicio emoção rage
     if (emotion_name == "rage") // rage
	{
         pitch = "-70%";
         range = "120%";
         rate = "95%"; 
         volume = "95%";
         contour = "(0%,-10%) (50%,-10%) (80%,-10%)";
	}
  else if (emotion_name == "annoyance") // annoyance
	{
         pitch = "-50%";
         range = "120%";
         rate = "95%"; 
         volume = "95%";
         contour = "(0%,-10%) (50%,-10%) (80%,-10%)";
	}
  else if (emotion_name == "anger") // anger
	{
         pitch = "-60%";
         range = "120%";
         rate = "95%"; 
         volume = "95%";
         contour = "(0%,-10%) (50%,-10%) (80%,-10%)";
	}
  //fim emoção rage
  //inicio emoção calma
  else if (emotion_name == "idle") // calm 
	{
         pitch = "0%";
         range = "0%";
         rate = "80%";
         volume = "60%";
         contour = "(50%)";
	}
  //fim emoção calma
  //inicio emoção Terror
  else if (emotion_name == "terror") // fear
	{
         pitch = "+100%";
         range = "20%";
         rate = "100%";
         volume = "70%";
         contour = "(50%, 100%)";
	}
 else if (emotion_name == "fear") // fear
	{
         pitch = "+70%";
         range = "20%";
         rate = "100%";
         volume = "70%";
         contour = "(50%, 100%)";
	}
 else if (emotion_name == "apprehension") // fear
	{
         pitch = "+50%";
         range = "20%";
         rate = "100%";
         volume = "70%";
         contour = "(50%, 100%)";
	}
  //fim emoção Terror

  //inicio emoção ecstasy
  else if (emotion_name == "ecstasy") // heppiness
	{
         pitch = "+75%";
         range = "200%";
         rate = "90%";
         volume = "80%";
         contour = "(0%, -5%) (80%, +25%)";
	}
  else if (emotion_name == "joy") // heppiness
	{
         pitch = "+50%";
         range = "200%";
         rate = "90%";
         volume = "80%";
         contour = "(0%, -5%) (80%, +25%)";
	}
  else if (emotion_name == "serenity") // heppiness
	{
         pitch = "+40%";
         range = "200%";
         rate = "90%";
         volume = "80%";
         contour = "(0%, -5%) (80%, +25%)";
	}
  //fim emoção ecstasy
  //inicio emoção grief
  else if (emotion_name == "grief") // sadness
	{
         pitch = "-30%";
         range = "70%";
         rate = "50%";
         volume = "40%";
         contour = "(0%, 100%) (80%, -10%)";
	}
  else if (emotion_name == "sadness") // sadness
	{
         pitch = "-20%";
         range = "70%";
         rate = "50%";
         volume = "40%";
         contour = "(0%, 100%) (80%, -10%)";
	}
  else if (emotion_name == "pensiveness") // sadness
	{
         pitch = "-10%";
         range = "70%";
         rate = "50%";
         volume = "40%";
         contour = "(0%, 100%) (80%, -10%)";
	}  
  //fim emoção grief

  //inicio emoção vigilance
  else if (emotion_name == "vigilance") // 
	{
         pitch = "+5%";
         range = "160%";
         rate = "92%";
         volume = "87%";
         contour = "(0%,-10%) (50%,-25%)";
	}
    else if (emotion_name == "antecipation") // 
	{
         pitch = "+0%";
         range = "160%";
         rate = "92%";
         volume = "87%";
         contour = "(0%,-10%) (50%,-25%)";
	} 
    else if (emotion_name == "interest") // 
	{
         pitch = "-20%";
         range = "160%";
         rate = "92%";
         volume = "87%";
         contour = "(0%,-10%) (50%,-25%)";
	}   
  //inicio emoção loathing
  else if (emotion_name == "loathing") // 
	{
         pitch = "-50%";
         range = "95%";
         rate = "71%";
         volume = "63%";
         contour = "(80%,-10%)";
	}
  else if (emotion_name == "disgust") // 
	{
         pitch = "-40%";
         range = "95%";
         rate = "71%";
         volume = "63%";
         contour = "(80%,-10%)";
	}
  else if (emotion_name == "boredom") // 
	{
         pitch = "-30%";
         range = "95%";
         rate = "71%";
         volume = "63%";
         contour = "(80%,-10%)";
	}
  //inicio emoção amazement
  else if (emotion_name == "amazement") // 
	{
         pitch = "+35%";
         range = "45%";
         rate = "75%";
         volume = "55%";
         contour = "(50%, 100%) (80%,-10%)";
	}
   else if (emotion_name == "surprise") // 
	{
         pitch = "+25%";
         range = "45%";
         rate = "75%";
         volume = "55%";
         contour = "(50%, 100%) (80%,-10%)";
	}
   else if (emotion_name == "distraction") // nao sei para que usar isso, conversar com andre depois
	{
         pitch = "+20%";
         range = "45%";
         rate = "75%";
         volume = "55%";
         contour = "(50%, 100%) (80%,-10%)";
	}
  //inicio emoção admiration
  else if (emotion_name == "admiration") // 
	{
         pitch = "+87%";
         range = "110%";
         rate = "95%";
         volume = "75%";
         contour = "(50%, 100%) (0%, -5%) (80%, +25%)";
	}
  else if (emotion_name == "trust") // 
	{
         pitch = "+60%";
         range = "110%";
         rate = "95%";
         volume = "75%";
         contour = "(50%, 100%) (0%, -5%) (80%, +25%)";
	}
  else if (emotion_name == "acceptance") // 
	{
         pitch = "+45%";
         range = "110%";
         rate = "95%";
         volume = "75%";
         contour = "(50%, 100%) (0%, -5%) (80%, +25%)";
	}
}

int main (int argc, char** argv) {
  srand(time(NULL));
  ros::init(argc, argv, "voztts"); //Initialise and create a ROS node
  ros::NodeHandle nh_public, nh_private("~");
  // get params
  std::string engine_str = "mary";
  nh_public.param("language", _language, _language);
  nh_private.param("engine", engine_str, engine_str);
  _engine = ENGINE_MARYTTS;
  // make subscribers
  ros::Subscriber sub = nh_public.subscribe("/emotion", 1000, callback);
  ros::Subscriber tts_sub = nh_public.subscribe("tts", 1, tts_cb);
  ROS_INFO("language:'%s', engine:'%s', listening to:'%s'",
           _language.c_str(), engine_str.c_str(), tts_sub.getTopic().c_str());
  ros::spin();
}
