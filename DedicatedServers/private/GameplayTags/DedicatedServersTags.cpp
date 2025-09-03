#include "GameplayTags/DedicatedServersTags.h"


namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{


		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, "DedicatedServersTags.GameSessionsAPI.ListFleets", "List Fleets resource on the GameSessions API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, "DedicatedServersTags.GameSessionsAPI.FindOrCreateGameSession", "Find or Create GameSession resource on the GameSessions API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession, "DedicatedServersTags.GameSessionsAPI.CreatePlayerSession", "Create PlayerSession resource on the GameSessions API")
			
	}

	namespace LoginAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignUp, "DedicatedServersTags.LoginAPI.SignUp", "Creates a new User in the Login API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConfirmSignUp, "DedicatedServersTags.LoginAPI.ConfirmSignUp", "Confirms a new user in Login API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignIn, "DedicatedServersTags.LoginAPI.SignIn", "Retrieves Access Token, Id Token, and Refresh Token in Login API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignOut, "DedicatedServersTags.LoginAPI.SignOut", "Signs user out and invalidates tokens in Login API")
	}

	namespace GameStatsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RecordMatchStats, "DedicatedServersTags.GameStatsAPI.RecordMatchStats", "Records the stats of a match in Game Stats API")

		
	}
}