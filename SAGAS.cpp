#include <string.h>
#include <fstream> //読み込み
#include <iostream> // cout, endl, cin
#include <algorithm> // min, max, swap, sort, reverse, lower_bound, upper_bound
#include <map> // map
#include <set> // set
#include <iterator>
#include <random> 
#include <vector> //vector
#include <tuple> // tuple, make_tuple
#include <utility> // pair, make_pair
#include <cstdio> // printf


//時間取得
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <iomanip>
using namespace std;

#define lli long long int
#define rep(i, n) ;for (int i = 0;i < (int)(n);i++)
#define rep2(i, s, n) ;for (int i = s; i < (int)(n);i++)
#define ALL(vec)  (vec).begin(),(vec).end()
#define pi 3.1415926535897932384626433832795
#define MAX_INF 9223372036854775807
#define MIN_INF (9223372036854775807+1)
#define sosuu 1000000007
#define rand() randxor()
static bool debug_mode = false;
static double Passed_time = 0;
static int SA_msec = 10000;
//string Y = "200031419105400422";
static double waste_heat = 1;
static int M = 0, K = 0, J = 0, bt=1;

static int sum_of_operation = 0;  
static int NMAX= 1000000000;

static int x=136436069,y=362436069,z=521288629,w=88675123;

int randxor()
{
  int t;
  t=(x^(x<<11));x=y;y=z;z=w; return( (w=(w^(w>>19))^(t^(t>>8)))% NMAX);
}

int schedule_limit= 10800000;


static lli score = 0;
static int SUP = -1;
static int INF = schedule_limit;
static int last_job_time_V=-1, first_job_time_V=schedule_limit;
static int a, b, c, d; 

static vector<vector<pair<int, int>>> Booking;

static vector<pair<int,int>> MachineShift;

static vector<int> EndTime;
lli getScore(vector<int> &N,vector<vector<int>> &current_list,vector<vector<int>> &tau,vector<int> &Dep_and_Order_num,  vector<vector<tuple<int, int, int, int>>> &Dep_and_Order,  vector<vector<int>> &C, vector<int> &MachineType_num)
{
  //Adding

  score = 0;
  SUP = -1;
  INF = schedule_limit;
  last_job_time_V=-1;
  first_job_time_V=schedule_limit;
  rep(j, J){
    rep(i, N[j]){
      last_job_time_V = max(max(SUP,last_job_time_V ),  current_list[j][i]+tau[j][i]);
      first_job_time_V = min(min(INF,first_job_time_V), current_list[j][i]);
    }


  }
  score = last_job_time_V-first_job_time_V;


  rep(j, J){
    rep(o, Dep_and_Order_num[j]){
      tie(a,b, c,d) = Dep_and_Order.at(j).at(o);
      lli dis = max(0, max(current_list[j][a]+c-(current_list[j][b]), current_list[j][b]-current_list[j][a]-d));
      dis*=dis;
      score += (lli)((lli)schedule_limit*(lli)waste_heat*(lli)waste_heat*(lli)dis);//シンプルにオーバーしてる時間分減点
      if(score < 0) return MAX_INF;
    }

  }


  Booking.clear();
  Booking.resize(K);
  rep(j, J){
    rep(o, N[j]){
      Booking.at(C[j][o]-1).push_back(make_pair(current_list[j][o], current_list[j][o]+tau[j][o]));
    }
  }

  rep(i, K) sort(ALL(Booking[i]));


  rep(i, K){
    MachineShift.clear();
    rep(j, Booking[i].size()){
      MachineShift.push_back(make_pair(Booking[i][j].first, 1));
      MachineShift.push_back(make_pair(Booking[i][j].second+bt, -1));
    }

    sort(ALL(MachineShift));

    EndTime.resize(Booking[i].size());
    int Booking_num=0;//同時使用人数
    int beftime=0;//直前の時間


    //______________________________________________________________________________________________ここ作る
    rep(j, MachineShift.size()){
      if(j==0){
        Booking_num += MachineShift[j].second;
        beftime=MachineShift[j].first; 

        continue;             
      }

      score+=(lli)((lli)schedule_limit*(lli)waste_heat*(lli)waste_heat*(lli)max(0, (Booking_num - MachineType_num[i])*(MachineShift[j].first-beftime)));
      // ここでインターバルのペナルティも入れておくとよきかな（最大機械数の場合, 減少後の再増加の間隔にペナルティ（どう頑張ってもインターバルを確保できないから））

      Booking_num += MachineShift[j].second;
      beftime=MachineShift[j].first;
      if(score < 0) return MAX_INF;
    }

  }

  if(score < 0) return MAX_INF;
  return score;
}

void Booking_List_Generator(vector<vector<int>> &C_ans, vector<int> &N,vector<vector<int>> &current_list,vector<vector<int>> &tau,  vector<int> T,  vector<vector<int>> &C)
{

  int Machine_Type; 
  int BegTime;//開始時間
  int FinTime;//終了時間
 

vector<vector<tuple<int, int, int>>> List_For_Booking(K);
vector<vector<tuple<int, bool, int>>> LFB_A(K);  
vector<vector<pair<bool, int>>> Booking_machine(K);
 /*
  List_For_Booking.clear();
  LFB_A.clear();
  Booking_machine.clear();
  List_For_Booking.resize(K);
  LFB_A.resize(K);//[Machine_Type] =  <Time, 開始時刻かどうか(false なら 終了時刻), List_For_Booking[Machine_Number].at()の部分、すなわちList_For_Booking[Machine_Number]におけるどのジョブか>
  Booking_machine.resize(K);//<現在booking されているか, Booking_Machine_num>
 */
  if(debug_mode) cout << "Cleared" << endl;
  if(debug_mode) cout << "M: "  << M<< endl;

  rep(i, M){
if(debug_mode) cout << "T[i]: " << T[i]<<  endl;

  }
  rep(i, M){
    Booking_machine.at(T[i]).push_back(make_pair(false, i));

  }
  if(debug_mode) cout << "Booking_machine Cleared" << endl;

  rep(k, K){

    if(debug_mode) cout << "k: " << k << endl;
    rep(BM, Booking_machine[k].size()){
      if(debug_mode) cout << "   Machine: " << Booking_machine[k][BM].second << " Booked?: " << Booking_machine[k][BM].first <<endl;


      Booking_machine[k][BM].first = false;
    }}



  rep(j, J){
    rep(o, N[j]){
      Machine_Type = C[j][o]-1;//機械の絶対的な番号を保持0~K-1
      List_For_Booking[Machine_Type].push_back(make_tuple(j, o, 0));//jobnum, openum を取得
      BegTime =current_list[j][o];//開始時刻
      FinTime =current_list[j][o]+tau[j][o]+bt;//終了時刻
      LFB_A[Machine_Type].push_back(make_tuple(BegTime, true, List_For_Booking[Machine_Type].size()-1));//開始時刻を入れる ＆List_For_Booking[Machine_Number]における立ち位置を指定
      LFB_A[Machine_Type].push_back(make_tuple(FinTime, false, List_For_Booking[Machine_Type].size()-1));//終了時刻を入れる ＆List_For_Booking[Machine_Number]における立ち位置を指定
    }

  }
  int Booking_Time, List_Num;
  bool Start_Or_Not;

  rep(i, K){
    if(debug_mode) cout << "K: " << i << endl;
    rep(j, List_For_Booking[i].size()){
      int asd, adf, afg;
      tie(asd, adf, afg) =List_For_Booking[i][j];
    if(debug_mode) cout << "        j: " << asd << "  o: " << adf << "  0: " << 0 << endl;

    }

  }


  rep(k, K){//種類k について行う
    if(debug_mode) cout << "k: " << k << endl;
    sort(ALL(LFB_A[k]));
    rep(i, LFB_A[k].size()){
      tie(Booking_Time,Start_Or_Not, List_Num) =LFB_A[k][i];
      if(Start_Or_Not){// start ならば（開始時刻）
        int Job, Ope, hoge;
        tie(Job, Ope, hoge) =List_For_Booking[k][List_Num];//Job及びOperation ナンバーを取得し、（hogeは利用しない）
        rep(BM, Booking_machine[k].size()){//予約できる機械を探す
          if(Booking_machine[k][BM].first) ;//bookingされていたらスルー
          else{//bookingされていないなら、
            C_ans[Job][Ope] = Booking_machine[k][BM].second+1;//答えに代入
            Booking_machine[k][BM].first=true;//予約されている状況に
            List_For_Booking[k][List_Num] = make_tuple(Job, Ope, BM);//種類k の BM 番目を予約したことをメモ
            break;//このブレイクを忘れてた
          }

        }
      }
      else{//終了時刻ならば
        int Job, Ope, BM;
        tie(Job, Ope, BM) =List_For_Booking[k][List_Num];// 種類k, BM 番目 の機械の予約を解除したい
        Booking_machine[k][BM].first=false;//生成アルゴリズムが間違えている?//予約を解除した状態にする。
      }
      rep(BM, Booking_machine[k].size()){

      if(debug_mode) cout << "   Machine: " << Booking_machine[k][BM].second << " Booked?: " << Booking_machine[k][BM].first << " ";
      }
    if(debug_mode) cout << endl;
    }
  }


}




bool isNumber(const string& str)
{
  for (char const &c : str) {
    if (std::isdigit(c) == 0) return false;
  }
  return true;
}


void input_Jobs(string filename, vector<int> &N, vector<vector<int>> &C, vector<vector<int>> &tau){

  vector<int> numbers;
  int number;
  ifstream input_file(filename);
  if (!input_file.is_open()) {
    cout << "Could not open the file - '"
      << filename << "'" << endl;
  }
  string F;
  rep(j, 5){
    input_file >> F;
  }
  int count = 0;
  while (!input_file.eof())
  {
    input_file >> F;

    if(input_file.eof()) break;
    if(isNumber(F)){
      int Job_ID = stoi(F);
      int Operation_ID, Compatible_machine, Processing_time;
      input_file >> Operation_ID;
      input_file >> Compatible_machine;
      input_file >> Processing_time;    
      sum_of_operation ++ ;
      while(N.size()<Job_ID){


        J++;
        N.push_back(0);
        C.push_back(vector<int>());
        tau.push_back(vector<int>());
      }
      N[Job_ID-1] ++;
      C[Job_ID-1].push_back(Compatible_machine);
      tau[Job_ID-1].push_back(Processing_time);
      count ++;
    }
    else{
    }



  }

  input_file.close();
}


void input_Machine(string filename, vector<int> &T, vector<int> &MachineType_num){
  std::set<int> s;

  int number;
  ifstream input_file(filename);
  if (!input_file.is_open()) {
    cout << "Could not open the file - '"
      << filename << "'" << endl;
  }
  string F;
  rep(j, 3){
    input_file >> F;
  }
  int count = 0;
  while (!input_file.eof())
  {
    input_file >> F;

    if(input_file.eof()) break;
    if(isNumber(F)){
      int Machine_ID = std::stoi(F);
      int Type;
      input_file >> Type;

      while(MachineType_num.size() < Type){
        MachineType_num.push_back(0);

      }
      T.push_back(Type-1);   
      MachineType_num[Type-1]++;
      s.insert(Type-1);

    }
    count ++;
  }
  M=T.size();
  K = s.size();
  input_file.close();

}


void input_Dependency(string filename, vector<int> &D, vector<vector<pair<int, int>>> &dependency){



  vector<int> numbers;
  int number;
  ifstream input_file(filename);
  if (!input_file.is_open()) {
    cout << "Could not open the file - '"
      << filename << "'" << endl;
  }

  int Job_ID, Operation_ID_1, Operation_ID_2;
  string F;

  rep(j, 3){
    input_file >> F;
  }
  while (!input_file.eof())
  {
    input_file >> Job_ID;

    if(input_file.eof()) break;
    input_file >> Operation_ID_1;
    input_file >> Operation_ID_2;
    D[Job_ID-1]++;
    dependency[Job_ID-1].push_back(make_pair(Operation_ID_1, Operation_ID_2));


  }

  input_file.close();
}


void input_TCMB(string filename, vector<int> &TCMB_num, vector<vector<tuple<int, int, int, int>>> &TCMB){



  vector<int> numbers;
  ifstream input_file(filename);
  if (!input_file.is_open()) {
    cout << "Could not open the file - '"
      << filename << "'" << endl;
  }

  int Job_ID, Operation_ID_1, Operation_ID_2, Time_constraint;
  string P1, P2, F;
  int Type;

  rep(j, 6){
    input_file >> F;
  }
  while (!input_file.eof())
  {
    input_file >> Job_ID;

    if(input_file.eof()) break;
    input_file >> Operation_ID_1;
    input_file >> P1;
    input_file >> Operation_ID_2;
    input_file >> P2;
    input_file >> Time_constraint;
    if(P1=="end"&&P2=="end") Type=4;
    if(P1=="start"&&P2=="end") Type=3;
    if(P1=="end"&&P2=="start") Type=2;
    if(P1=="start"&&P2=="start") Type=1;
    TCMB_num[Job_ID-1]++;
    TCMB.at(Job_ID-1).push_back(make_tuple(Type,Operation_ID_1, Operation_ID_2, Time_constraint));
  }

  input_file.close();
}

bool checkFileExistence(const std::string& str)
{
  std::ifstream ifs(str);
  return ifs.is_open();
}

string getDatetimeStr() {
  time_t t = time(nullptr);
  const tm* localTime = localtime(&t);
  std::stringstream s;
  s << localTime->tm_year + 1900;
  s << "_";
  s << setw(2) << setfill('0') << localTime->tm_mon + 1;
  s << "_";
  s << setw(2) << setfill('0') << localTime->tm_mday;
  s << "_";
  s << setw(2) << setfill('0') << localTime->tm_hour;
  s << "_";
  s << setw(2) << setfill('0') << localTime->tm_min;
  s << "_";
  s << setw(2) << setfill('0') << localTime->tm_sec;

  return s.str();
}

void make_log(string &Output_directory,   string &Experimental_ID,  time_t &Ex_start, int SA_msec, map<string, string> &Conditions, map<string, string> command_line){

  ofstream Log_file;
  string DATE_now = getDatetimeStr();
  string fileName_an = Output_directory +Experimental_ID + "_"+DATE_now + "_log.txt";

  string Input_words;
  if(!checkFileExistence(fileName_an))  {
    Log_file.open(fileName_an, std::ios::out);
    Log_file << "Log "<<  "\n";
    Log_file <<"DATE(GMT): YYYY_MM_DD_hh_mm_ss\n";
    Log_file <<"DATE(GMT): " <<DATE_now  << "\n";

    Log_file << "Experimental_ID: " << Conditions["ID"]  << "\n";
    for (const auto &item : Conditions) {
        if(item.first == "ID" ) continue;
        if(item.first == "DATE(GMT)" ) continue;
        Log_file << item.first << ": " << item.second << "\n";
    }
    if(command_line.count("-log") == 0){
    
    cout << "Type your name: " << endl;
    cin >>  Input_words;
    }

     
    else     Input_words = command_line["log_writer"];
    Log_file << "User name: " << Input_words << "\n";
  Log_file << "\nComments\n";
    Log_file.close();
  }
  Log_file.open(fileName_an, std::ios::app);


    if(command_line.count("-log") == 0){
    
  cout << "Type your comments below me. (If you finish this, please enter 'QUIT'. )" << endl;
  cin >> Input_words;
  while (Input_words != "QUIT"){
    Log_file << Input_words<< "\n";
    cin >> Input_words;
  }
    }
    else{
Log_file << command_line["log_comment"]<< "\n";

    }
    Log_file <<"DATE(GMT): " <<getDatetimeStr()  << "\n\n";



  Log_file.close();


  std::cout << fileName_an << "に書き込みました。" << std::endl;


}

void make_config(string &Output_directory,  string &Experimental_ID,  time_t &Ex_start, int SA_msec, map<string, string> &Conditions ){

  ofstream Config_file;
  string fileName_config = Output_directory +Experimental_ID+"_config.tsv";
    Config_file.open(fileName_config, std::ios::out);
    Config_file <<"DATE(GMT)\t" <<getDatetimeStr()  << "\n";

    for (const auto &item : Conditions) {
        Config_file << item.first << "\t" << item.second << "\n";
    }


  Config_file.close();


  std::cout << fileName_config << "に書き込みました。" << std::endl;


}

void Input_config(string filename, map<string, string> &Conditions){
    string Condition_name, Condition_data;
    ifstream input_file(filename);
  if (!input_file.is_open()) {
    cout << "Could not open the file - '"
      << filename << "'" << endl;
  }
  while (!input_file.eof())
  {
    input_file >>  Condition_name >> Condition_data;
   cout << Condition_name << "\t" <<  Condition_data << endl;
   Conditions[Condition_name] = Condition_data;


  }

  input_file.close();

}



//途中経過のgreedyの判定をします。
bool JudgeM_for_greedy_failed(int Js/*Jのストップ*/, 
                              int quito/*今決定している最後のオペレーション*/ , 
                              vector<vector<set<int>>> &Deped_list,
                              vector<vector<vector<tuple<int, int, int>>>> &Depping_list,
                            vector<vector<vector<pair<int, int>>>> &Strict_list_for_greedy,  
                              vector<vector<int>> &greedy_list, 
                              vector<int> &N, 
                              vector<vector<int>> &tau,
                              vector<int> &Dep_and_Order_num,  
                              vector<vector<tuple<int, int, int, int>>> &Dep_and_Order,  
                              vector<vector<int>> &C, 
                              vector<int> &MachineType_num,
                              bool &Force_finish)
{

  score = 0;
  SUP = -1;
  INF = schedule_limit;
  last_job_time_V=-1;
  first_job_time_V=schedule_limit;


  Booking.clear();
  Booking.resize(K);
  rep(j, Js+1){
    rep(o, N[j]){
      Booking.at(C[j][o]-1).push_back(make_pair(greedy_list[j][o], greedy_list[j][o]+tau[j][o]));
      if(o == quito&&j==Js) break;//現在決定しているオペレーションまで終了したらquit
    }
  }

  rep(i, K) sort(ALL(Booking[i]));


  rep(i, K){
    MachineShift.clear();
    rep(j, Booking[i].size()){
      MachineShift.push_back(make_pair(Booking[i][j].first, 1));
      MachineShift.push_back(make_pair(Booking[i][j].second+bt, -1));
    }

    sort(ALL(MachineShift));

    EndTime.resize(Booking[i].size());
    int Booking_num=0;//同時使用人数
    int beftime=0;//直前の時間


    //______________________________________________________________________________________________ここ作る
    rep(j, MachineShift.size()){
      if(j==0){
        Booking_num += MachineShift[j].second;
        beftime=MachineShift[j].first; 

        continue;             
      }

      score+=(lli)((lli)schedule_limit*(lli)waste_heat*(lli)waste_heat*(lli)max(0, (Booking_num - MachineType_num[i])*(MachineShift[j].first-beftime)));
      Booking_num += MachineShift[j].second;
      beftime=MachineShift[j].first;
      if(score != 0) return true;
    }

  }

  if(score != 0) return true;

  return false;
}



void Greedy(
    int &J, 
                      vector<vector<set<int>>> &Deped_list,
                      vector<vector<vector<tuple<int, int, int>>>> &Depping_list,
                      vector<vector<vector<pair<int, int>>>> &Strict_list_for_greedy,  
                      vector<vector<int>> &greedy_list, 
                      vector<int> &N, 
                      vector<vector<int>> &tau,
                      vector<int> &Dep_and_Order_num,  
                      vector<vector<tuple<int, int, int, int>>> &Dep_and_Order,  
                      vector<vector<int>> &C, 
                      vector<int> &MachineType_num,
                      bool &Force_finish,
                      int greedy_time_limit,
                      time_t greedy_start
){
  struct timeval time_now{};
  gettimeofday(&time_now, nullptr);
  time_t greedy_now = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
    int j_g = 0;
  int o_g = 0;
  int loopcount = 0;
      {
    Greedy_nextloop:
    loopcount ++;

      if(greedy_time_limit != -1 && loopcount %100 == 1){
  gettimeofday(&time_now, nullptr);
  time_t greedy_now = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
  if(greedy_now - greedy_start > greedy_time_limit){
        Force_finish = true;
        printf("Greedy time over.\nLimit is %d but passed time is %d (ms).\n", greedy_time_limit, (int)(greedy_now - greedy_start));
      return ;
  }

      }

    if(Force_finish){
      return ;
    }//不可能な場合
    //[j_g][o_g]に関してのサーチを行う
    //制約を確認する。Strict_list_for_greedy[j_g][o_g][i]=依存先O_dep, O_depに関して守らなければならない制約の下限, O_depに関して守らなければならない制約の上限
    /*
     例えば、O_depの終了時間が20, O＿depの終了時間に対して5分以内に作業を始めなければならないのなら、
     Strict_list_for_greedy[j_g][o_g][i] = {O_dep, 15, 25};
     */
    if(debug_mode) cout << "strict_check..." << endl;

    int O_dep, time1 = 0, time2 = schedule_limit;
    int O_dep_C, time1_C, time2_C;
    rep(i, Depping_list[j_g][o_g].size()){
      tie(O_dep, time1_C, time2_C) = Depping_list[j_g][o_g][i];
      time1_C += greedy_list[j_g][O_dep];
      time2_C += greedy_list[j_g][O_dep];

      if(debug_mode) cout << "strict_check..." << endl;
      Strict_list_for_greedy[j_g][o_g][O_dep] = make_pair(time1_C, time2_C);
      if(debug_mode) cout << "strict_check..." << endl;

      time1 = max(time1, time1_C);
      time2 = min(time2, time2_C);

      if(debug_mode) cout << "strict_check..." << endl;
    }
    if(debug_mode) cout << "strict check done" << endl;

    //次に、制約の範囲内で小さい順に機器制約を満たすものがあるかを確認する
    bool passed=false;
    int Js = j_g;

    int quito = o_g;
    if(debug_mode) cout << "j_g: " << j_g << ", o_g: " << o_g << "\t" << time1 << "~" << time2 << endl;
    if(debug_mode) cout << "searching...\t";
    rep2(time_candidate, time1, time2+1){
      greedy_list[j_g][o_g] = time_candidate;



      if(JudgeM_for_greedy_failed(Js/*Jのストップ*/,  quito/*今決定している最後のオペレーション*/ ,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish)/*機械の使用制約に違反する*/){
        continue;
      }
      else{      if(debug_mode) cout << "...\t done!\n";
           //全部のチェックに通った
           //次の作業へ//
           passed = true;
           //時間を記録
           break;
          }

    }
    if(passed){
      if(debug_mode) cout << "set\tj_g:" << j_g << ",\to_g:" << o_g << "\ttime:" << greedy_list[j_g][o_g] << endl;;
      //次の探索へ移る
      if(o_g == greedy_list[j_g].size()-1/*最終オペレーション*/){
          if(debug_mode)cout <<"Greedy... job[" << j_g << "] done!"  << endl;

        if(j_g == greedy_list.size()-1){
          goto Greedy_end;
          //終わり
        }
        else{
            
          j_g++;
          if(debug_mode)cout <<"Greedy... job[" << j_g << "] next!"  << endl;
          o_g = 0;
          goto Greedy_nextloop;
          
        }
      }
      else{
        if(debug_mode) cout << "next" << endl;
        o_g++;
        goto Greedy_nextloop;

      }
    }
    else{//not passed
      //何をUpdateするかを決定する
      if(Deped_list[j_g][o_g].size() == 0){
        Force_finish = true;
        if(debug_mode)cout << "GREEDY FAILED" << endl;
        rep(j_, J){
          rep(o_, greedy_list[j_].size()){

            greedy_list[j_][o_] = schedule_limit;
          }
        }
          goto Greedy_nextloop;
      }
      else{





        //tie(O_dep_change, time1_change, time2_change) =  Strict_list_for_greedy[j_g][o_g][i];
        if(debug_mode) cout << "FAIL searching" << endl;
        rep(i, Depping_list[j_g][o_g].size()){
          if(debug_mode) cout << O_dep << "\t" << time1_C << "\t" << time2_C << endl;
          tie(O_dep, time1_C, time2_C) = Depping_list[j_g][o_g][i];
          time1_C += greedy_list[j_g][O_dep];
          time2_C += greedy_list[j_g][O_dep];
          if (time2 ==  time2_C){
            o_g = O_dep;
            goto Updating_greedy_node;
            //Update_node_for_greedy(j_g, O_dep,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish, greedy_time_limit, greedy_start);

            //違反の原因となる依存先に飛ぶ
            //Greedy_searching(j_g, o,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish);

          }
        }

        //___________________________________________________Update先の指定をする
      }
    }





  }
  

  Updating_greedy_node:
  {
  int O_dep, time1 = 0, time2 = schedule_limit;
  int O_dep_C, time1_C, time2_C;
  rep(i, Depping_list[j_g][o_g].size()){
    tie(O_dep, time1_C, time2_C) = Depping_list[j_g][o_g][i];
    time1_C += greedy_list[j_g][O_dep];
    time2_C += greedy_list[j_g][O_dep];
    Strict_list_for_greedy[j_g][o_g][O_dep] = make_pair(time1_C, time2_C);
    time1 = max(time1, time1_C);
    time2 = min(time2, time2_C);
  }
  bool passed = false;
  if(debug_mode) cout << greedy_list[j_g][o_g] << "->" << greedy_list[j_g][o_g]+1  << endl;
  time1 = max(greedy_list[j_g][o_g] + 1, time1);
  rep2(time, time1, time2 + 1/*制約の範囲内*/){

    if(time >= time2 + 1 ){//依存先の制約に違反する
      continue;
    }
    else{
      int Js = j_g;

      int quito = o_g;

      greedy_list[j_g][o_g] = time;
      if(debug_mode) cout << "greedy_list[" << j_g << "][" << o_g << "] = " << time << endl;

      if(JudgeM_for_greedy_failed(Js/*Jのストップ*/,  quito/*今決定している最後のオペレーション*/ ,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish)/*機械の使用制約に違反する*/){
        continue;
        //機器の制約違反
      }
      else{//全部のチェックに通った

        //あらためて元の作業へと戻る
        passed = true;
        o_g++;
        goto Greedy_nextloop;

        //Greedy_searching(j, o + 1,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish);
      }
    }

  }
  if(passed == false){



    //tie(O_dep_change, time1_change, time2_change) =  Strict_list_for_greedy[j_g][o_g][i];
    rep(i, Depping_list[j_g][o_g].size()){
      tie(O_dep, time1_C, time2_C) = Depping_list[j_g][o_g][i];
      time1_C += greedy_list[j_g][O_dep];
      time2_C += greedy_list[j_g][O_dep];
      if (time2 ==  time2_C){
            o_g = O_dep;
            goto Updating_greedy_node;
        
        //違反の原因となる依存先に飛ぶ
        //Greedy_searching(j, O_dep,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish);

      }
    }

    //___________________________________________________Update先の指定をする

  }


}
  

Greedy_end:
return;
}
static int giko = 0;
void Giko_comment(string comment){//もじれつのひかく
                    
comment += "\n";
string Giko;
if(giko%3 == 0){
       Giko = "     ＿＿＿_∧   ∧     ／￣￣￣￣￣￣￣￣\n"
              "  ～|＿＿__(,,゜Д゜)＜ " + comment+
              "    ＵU    Ｕ  U   ＼＿＿＿＿＿＿＿＿\n";}
if(giko%3 == 1){
       Giko = "     ＿＿＿_∧  ∧     ／￣￣￣￣￣￣￣￣\n"
              "  ～|＿＿__(,゜Д゜)＜ "+ comment+
              "    ＵU    Ｕ  U   ＼＿＿＿＿＿＿＿＿\n";}
if(giko%3 == 2){
       Giko = "     ＿＿＿_∧ ∧     ／￣￣￣￣￣￣￣￣\n"
              "  ～|＿＿__(゜Д゜)＜ " + comment+
              "    ＵU    Ｕ  U   ＼＿＿＿＿＿＿＿＿\n";}
if(giko>10){
       Giko = "     ∧∧     ／￣￣￣￣￣\n"
              "    (,,ﾟДﾟ)＜" + comment+
              "   ⊂    ⊃   ＼＿＿＿＿＿\n"
              "  ～|    |\n"
              ",,   し`J\n";}
if(giko>20){
       Giko = "     ∧∧     ／￣￣￣￣￣\n"
              "    (,,-Д-)＜" + comment+
              "   ⊂    ⊃   ＼＿＿＿＿＿\n"
              "  ～|    |\n"
              ",,   し`J\n";}
if(giko>30){
       Giko = "     ＿＿＿_∧ ∧     ／￣￣￣￣￣￣￣￣\n"
              "  ～|＿＿__(-Д-)＜ " + comment+
              "           ⊃ ⊂    ＼＿＿＿＿＿＿＿＿\n";}

copy(Giko.begin(), Giko.end(),
ostream_iterator<char>(cout, ""));
cout << endl;
giko ++;
giko %= 50;


}

int main(int argc, char *argv[]){
    map<string, string> command_line;
    int greedy_time_limit= -1;


    int i=0;
    while(i<argc){
      string arg = argv[i];
      
      if(arg == "-d"){
          command_line[arg] = "";
                      }
      
      else if(arg == "-penalty"){
        i++;

      arg = argv[i];
          schedule_limit = stoi(arg);
                      }

      else if(arg == "-gl"){
        i++;
      arg = argv[i];
      greedy_time_limit = stoi(arg);
                      }
      
      else if(arg == "-m"){
          command_line[arg] = "";
                      }
      else if(arg == "-fs"){
          command_line[arg] = "";
                      }
      else if(arg == "-log"){
          
          command_line[arg] = "";
          i++;
          command_line["log_writer"] = argv[i];
          i++;
          command_line["log_comment"] = argv[i];
                      }
      
      else if(arg == "-conf"){
          i++;
          string conf = argv[i];
          command_line[arg] = conf;
                      }
      else if(strcmp(argv[i], "-help!") == 0){//もじれつのひかく
          string help = "Help! I need somebody\n"
  "Help! Not just anybody\n"
  "Help! You know I need someone Help!\n"
  "https://www.youtube.com/watch?v=2Q_ZzBGPdqE"
  ;
      copy(help.begin(), help.end(),
          std::ostream_iterator<char>(cout, ""));
      cout << endl;

      }
      else if(strcmp(argv[i], "-giko") == 0){
          Giko_comment("Giko mode!" );
        command_line[argv[i]]++;

      }
                  i++;
      }
    



  struct timeval time_now{};
  gettimeofday(&time_now, nullptr);
  time_t Ex_start = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
   if(debug_mode) cout << "Start_time is " << Ex_start << endl;
  string Experimental_ID = "ID" + to_string(Ex_start);
  string test_time_ = "test_time_";
  string Output_directory;
  string test_dep_ = "test_dep_";
  string test_machines_ = "test_machines_";
  string test_result_ = "test_result";
  string tsvtxt = "_" + Experimental_ID + ".tsv";
  string fileName_time;
  string fileName_dep;
  string fileName_mac;
  string fileName_res;
  string the_date = getDatetimeStr() ;
  string Source_directory;
  char Ans;
  int MAC_COUNT;

  int Thr;
  double Th;
  bool Force_next;
  bool Local_fitting_J1 = true;
  bool Local_fitting_J2 = true;
  bool Local_fitting_J3 = true;
  bool Local_fitting_J1_updated = true;
  bool Local_fitting_J2_updated = true;
  bool Local_fitting_J3_updated = true;
  int Set_end_point = 0;
  bool Started_from_the_middle = true;
  int Set_start_from = 0;
  int Loop_start_from = 0;
  int Loop_end_point = 0;
  bool End_the_middle = true;
  int last_time_start = 0;
  time_t greedy_start = 0;
  bool From_greedy = false;
  bool With_greedy = false;
  bool Align_start = false;
  bool Strict_adjustment = false;
  bool Make_log_question = false;
      bool adopt_greedy = false;
      bool Force_finish = false;
bool SA_naive = false;

  bool FAIL = false;
  bool Having_config = false;
  bool Giko_mode = (command_line.count("-giko") == 1);
  vector<int> MachineType_num(0);
  vector<int> T(0);
  vector<int> N(0);
  vector<vector<int>> C(0);
  vector<vector<int>> tau(0);
  vector<int> D(0);
  vector<vector<pair<int, int>>> dependency(0); 
  vector<int> TCMB_num(0);
  vector<vector<tuple<int, int, int, int>>> TCMB(0);
  vector<int> MachineType_num_keep(0);
  vector<int> T_keep(0);
  int M_keep;
  vector<int> ADD_base(0);
  vector<int> ADD_cal(0);
  vector<int> ADD_bit(0);
  int RESERVE_NUM;
  int Loops ;
  int Result_num;
  srand((int)time(NULL));
  x=rand();
  int Machine_max_num=0;
  debug_mode = (command_line.count("-d") == 1);

  map<string, string> Conditions;
  while(true){
  if(command_line.count("-m") == 0){
    Having_config = true;
      Input_config(command_line["-conf"], Conditions);
  Source_directory = Conditions["Source_directory"];
  Output_directory = Conditions["Output_directory"];
  Output_directory += "/";
  cout << Output_directory << endl;

break;
}
  cout << "Do you have config?"<< flush;
    cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
        Having_config = true;
        string Config_path;
        cout << "Input the path:\t" << flush;
        cin >>  Config_path;
        Input_config(Config_path, Conditions);
        
      break;
    }
    else if(Ans == 'n' || Ans == 'N'){
      break;
    }
  }
  
  //input000000000000000000000000000000
  string Machine_Source = Source_directory + "/machines.tsv";
  input_Machine(Machine_Source, T, MachineType_num);
  input_Jobs(Source_directory + "/operations.tsv", N, C, tau);

  //pairの受け取りから
  D.resize(J);
  dependency.resize(J); 
  input_Dependency(Source_directory + "/dependency.tsv", D, dependency);

  rep(i, J){
    sort(ALL(dependency[i]));
  }

  TCMB_num.resize(J);
  TCMB.resize(J);
  input_TCMB(Source_directory + "/tcmb.tsv", TCMB_num, TCMB);

  //input終わり000000000000000000000000000000
  //処理


  MachineType_num_keep = MachineType_num;
  T_keep = T;
  M_keep = M;
  Machine_max_num = M;
  ADD_base.resize(K, 0);
  ADD_cal.resize(K, 0);
  ADD_bit.resize(K, 0);
  RESERVE_NUM=M;
  Loops = 5;
  Result_num = 1;
if(!Having_config){
rep(i, K){
    Conditions["ADD_machine" + to_string(i+1)] = "0";
  }
Conditions["Adjust_strictly"] = "false";  
Conditions["Loops"] = "1";  
Conditions["Align_start_time"] = "false";
Conditions["Annealing_time_(msec)"] = "-1";
Conditions["End_the_middle"] = "false";
Conditions["From_greedy_searching"] = "false";
Conditions["ID"] = Experimental_ID;
Conditions["Loop_end_point"]="0";
Conditions["Loop_start_from"] = "0";
Conditions["Output_directory"] = "Output_directory";
Conditions["Set_end_point"]="0";
Conditions["Set_start_from"] = "-1";
Conditions["Source_directory"] = "Source_directory";
Conditions["Start_from_the_middle"] = "false";
Conditions["With_greedy_searching"] = "false";
}

if(Having_config){
    
rep(i, K){
    ADD_base[i] = stoi(Conditions["ADD_machine" + to_string(i+1)]);
  Result_num *=ADD_base[i]+1;
  RESERVE_NUM*=ADD_base[i]+1;
  Machine_max_num += ADD_base[i];
  }

  Strict_adjustment = (Conditions["Adjust_strictly"]=="true"||Conditions["Adjust_strictly"]=="True"||Conditions["Adjust_strictly"]=="TRUE");
  Align_start =(Conditions["Align_start_time"]=="true"||Conditions["Align_start_time"]=="True"||Conditions["Align_start_time"]=="TRUE");
  SA_msec = stoi(Conditions["Annealing_time_(msec)"]);
  From_greedy = (Conditions["From_greedy_searching"]=="true"||Conditions["From_greedy_searching"]=="True"||Conditions["From_greedy_searching"]=="TRUE");
  Experimental_ID = Conditions["ID"];
  With_greedy = (Conditions["With_greedy_searching"]=="true"||Conditions["With_greedy_searching"]=="True"||Conditions["With_greedy_searching"]=="TRUE");
  Loops = stoi(Conditions["Loops"]);
  if(Conditions.count("Naive") >= 1){
  SA_naive = (Conditions["Naive"]=="true"||Conditions["Naive"]=="True"||Conditions["Naive"]=="TRUE");
}
Conditions["Naive"] = "true";
if(Conditions.count("Start_from_the_middle")== 0){
  while(true){
    cout << "Start from the middle？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      cout << "If you want to start _0_LoopX (the beggining of loop X), Set next from?: -1, Loop next from?: X\nSet next from?" << std::flush;
      cin >> Set_start_from;
      cout << "Loop next from?" << std::flush;
      cin >> Loop_start_from;
Conditions["Start_from_the_middle"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      Started_from_the_middle = false;
      break;
    }
  }
  }
  else{
      
  Started_from_the_middle = (Conditions["Start_from_the_middle"]=="true"||Conditions["Start_from_the_middle"]=="True"||Conditions["Start_from_the_middle"]=="TRUE");
  if(Started_from_the_middle ){
      if(Conditions.count("Set_start_from")== 0){
      cout << "If you want to start _0_LoopX (the beggining of loop X), Set next from?: -1, Loop next from?: X\nSet next from?" << std::flush;
      cin >> Set_start_from;
      Conditions["Set_start_from"] = to_string(Set_start_from);
      }
      else Set_start_from = stoi(Conditions["Set_start_from"]);

      if(Conditions.count("Loop_start_from")== 0){
      cout << "Loop next from?" << std::flush;
      cin >> Loop_start_from;
      Conditions["Loop_start_from"] = to_string(Loop_start_from);
      }
      else Loop_start_from = stoi(Conditions["Loop_start_from"]);
  }
      
  }
  
if(Conditions.count("End_the_middle")== 0){

  while(true){
    cout << "End the middle？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      cout << "Set the end point?" << std::flush;
      cin >> Set_end_point;
      cout << "End loop number?" << std::flush;
      cin >> Loop_end_point;
Conditions["End_the_middle"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      End_the_middle = false;
      break;
    }
  }
}


  else{
      
  End_the_middle = (Conditions["End_the_middle"]=="true"||Conditions["End_the_middle"]=="True"||Conditions["End_the_middle"]=="TRUE");
  if(End_the_middle){
      if(Conditions.count("Set_end_point")== 0){
      cout << "Set the end point?" << std::flush;
      cin >> Set_end_point;
      Conditions["Set_end_point"]=Set_end_point;
      }
      else Set_end_point = stoi(Conditions["Set_end_point"]);
      
      if(Conditions.count("Loop_end_point")== 0){
      cout << "End loop number?" << std::flush;
      cin >> Loop_end_point;
      Conditions["Loop_end_point"]=Loop_end_point;
      }
      else Loop_end_point = stoi(Conditions["Loop_end_point"]);
      
  }
      
  }

  goto Config_input_end;


}

  while(true){
    cout << "Experimental ID を設定しますか？(y/n)"<< std::flush;
    cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      cout << "Set the ID: " << std::flush;
      cin >> Experimental_ID;
      Conditions["ID"] = Experimental_ID;
      break;
    }
    else if(Ans == 'n' || Ans == 'N'){
      break;
    }
  }

  cout << "Experimental_ID is " << Experimental_ID << endl;

  


  cout << "Input the directory having dependency.tsv, machines.tsv, operations.tsv and tcmb.tsv: ";
  cin >> Source_directory;
  cout << "Input the directory as output: ";
  cin >> Output_directory;
  Output_directory += "/";
  cout << "Input annealing time (msec): ";
  cin >> SA_msec;

Conditions["Annealing_time_(msec)"] = to_string(SA_msec);

  rep(i, K){
    cout << i+1 << " 種類目の機械をいくつまで増やしますか？" << std::flush;
    cin >> ADD_base[i];
    Result_num *=ADD_base[i]+1;
    RESERVE_NUM*=ADD_base[i]+1;
    Machine_max_num += ADD_base[i];
    Conditions["ADD_machine" + to_string(i+1)] = to_string(ADD_base[i]);
  }
  cout << "何ループ回しますか？" << std::flush;
  cin >> Loops;
  Conditions["Loops"] = to_string(Loops);



  while(true){
    cout << "Naive SA？(y/n)"<< std::flush;
    cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      SA_naive = true;
      Ans = 'n';
Conditions["Naive"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'){
      break;
    }
  }


  while(true){
    cout << "Start from the middle？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      cout << "If you want to start _0_LoopX (the beggining of loop X), Set next from?: -1, Loop next from?: X\nSet next from?" << std::flush;
      cin >> Set_start_from;
      Conditions["Set_start_from"] = to_string(Set_start_from);
      cout << "Loop next from?" << std::flush;
      cin >> Loop_start_from;
      Conditions["Loop_start_from"] = to_string(Loop_start_from);
Conditions["Start_from_the_middle"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      Started_from_the_middle = false;
      break;
    }
  }


  while(true){
    cout << "End the middle？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      cout << "Set the end point?" << std::flush;
      cin >> Set_end_point;
      Conditions["Set_end_point"]=to_string(Set_end_point);
      cout << "End loop number?" << std::flush;
      cin >> Loop_end_point;
      Conditions["Loop_end_point"]=to_string(Loop_end_point);
Conditions["End_the_middle"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      End_the_middle = false;
      break;
    }
  }



  while(true){
    cout << "From greedy searching？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      From_greedy = true;
Conditions["From_greedy_searching"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      From_greedy = false;
      break;
    }
  }
  


  while(true){
    cout << "With greedy searching？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      With_greedy = true;
Conditions["With_greedy_searching"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      With_greedy = false;
      break;
    }
  }

  



  while(true){
    cout << "Align start time？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      Align_start = true;
Conditions["Align_start_time"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      Align_start = false;
      break;
    }
  }


  while(true){
    cout << "Adjust strictly？(y/n)"<< std::flush;
    if(!SA_naive)cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      Strict_adjustment = true;
Conditions["Adjust_strictly"] = "true";
      break;
    }
    else if(Ans == 'n' || Ans == 'N'||SA_naive){
      Align_start = false;
      break;
    }
  }

  while(true){
    cout << "Make config？(y/n)"<< std::flush;
    cin >> Ans;
    if(Ans == 'y' || Ans == 'Y'){
      make_config(Output_directory, Experimental_ID, Ex_start, SA_msec, Conditions);
      break;
    }
    else if(Ans == 'n' || Ans == 'N'){
      break;
    }
  }


Config_input_end:

 tsvtxt = "_" + Experimental_ID + ".tsv";

  while(true){
        if(command_line.count("-log") == 1){
      Make_log_question = true;
      break;
  }
      
    cout << "Make log？(y/n)"<< std::flush;
    cin >> Ans;
    if(Ans == 'y' || Ans== 'Y'){
      Make_log_question = true;
      break;
    }
    else if(Ans == 'n' || Ans == 'N'){
      Make_log_question = false;
      break;
    }
  }





  Result_num *=Loops;
  vector<vector<int>> Result(Result_num, vector<int>(2 * K+1, 0));


  T.reserve(RESERVE_NUM*Loops);

  if(Make_log_question)
    make_log(Output_directory, Experimental_ID, Ex_start, SA_msec, Conditions, command_line);

  cout << "Result_num: "<< Result_num << endl;
  rep(ppp, K) ADD_base[ppp]++;

  rep(ppp, K){
    if(ppp==0) ADD_cal[ppp] = 1;
    else ADD_cal[ppp] =  (ADD_base[ppp-1]) *(ADD_cal[ppp-1]) ;
  }

  int line  = 1;
  rep(ppp, K){
    line *= ADD_base[ppp];

  }
  vector<vector<int>> C_ans = C;

  vector<vector<int>> best_list = C; //best な状態（同じ形なのでCで初期化）
  vector<vector<int>> greedy_list = C;
  vector<int> Dep_and_Order_num(J);
  vector<vector<tuple<int, int, int, int>>> Dep_and_Order(J);
  vector<vector<tuple<int, int, int, int>>> Dep_and_Order_ini(J);
  vector<vector<tuple<int, int, int>>> Dep_and_Order_current(J);
  vector<vector<tuple<int, int, int>>> Dep_and_Order_current_ini(J);
  vector<vector<int>> Dep_list = C;
  vector<vector<vector<pair<int, int>>>> Strict_list_for_greedy(J);
  vector<vector<vector<tuple<int, int, int>>>> Depping_list(J);
  vector<vector<set<int>>> Deped_list(J);
  map<pair<int, int>, bool> mapping;
  pair<int, int> obj;
  vector<int> Used_num_of_machine_types(K, 0);

  vector<vector<tuple<int, int, int>>> Dep_and_Order_next;

  vector<vector<int>> Next_list;

  vector<bool> Used_machines(M, false);
  Used_machines.reserve(Machine_max_num);


  rep(Loop, Loops){
    rep(ppp, line){
  gettimeofday(&time_now, nullptr);
  time_t Scheduling_start_sec = time_now.tv_sec;
  time_t Scheduling_start_msec = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
        
  greedy_list = C; //best な状態（同じ形なのでCで初期化）
  rep(j, J){
    Strict_list_for_greedy[j].resize(N[j]);
    rep(k, N[j]){
        Strict_list_for_greedy[j][k].resize(N[j]);
    }
  }


  rep(j, J){
    Deped_list[j].resize(N[j]);
  }
  rep(j, J){
    Depping_list[j].resize(N[j]);
  }
if(Giko_mode) Giko_comment("Loop:" +to_string(Loop)+", set:"+to_string(ppp));
else cout << "Loop:" +to_string(Loop)+", set:"+to_string(ppp) << endl;
      if(Started_from_the_middle){
        ppp = Set_start_from;
        Loop = Loop_start_from;
        Started_from_the_middle = false;
        continue;

      }

      if(End_the_middle){
        if(ppp >Set_end_point && Loop >= Loop_end_point) {
          ppp = line;
          Loop = Loops;
          continue;}
      }

      MachineType_num = MachineType_num_keep;
      if(debug_mode){rep(i, MachineType_num.size()) cout << MachineType_num[i]  << " " ;}
      T.resize(T_keep.size());
      M = M_keep;
      T = T_keep;

      rep(adding, K){
        ADD_bit[adding] = (ppp/ADD_cal[adding])%ADD_base[adding];
        M += ADD_bit[adding];
              }
      rep(adding, K){
        MachineType_num[adding] += ADD_bit[adding];
        Result[Loops * ppp + Loop][adding] = MachineType_num[adding];
        rep(AD, ADD_bit[adding]) T.push_back(adding);
      }
 


      FAIL = false;
      //初期状態の生成1111111111111111111

      best_list = C; //best な状態（同じ形なのでCで初期化）
      //vector<int> Dep_and_Order_num(J);
      Dep_and_Order=Dep_and_Order_ini;
      Dep_and_Order_current = Dep_and_Order_current_ini;
      Dep_list = C;  
  

  rep(j, J){
    rep(k, N[j]){
        rep(l, N[j]) Strict_list_for_greedy[j][k][l] = make_pair(0,schedule_limit);
    }
  }



      //Dep_and_Order.at(i).push_back(make_tuple(依存先(O_1), 依存者(O_2), 下限, 上限));

      int tcmb_type,O_1, O_2, alpha; 

      int last_job_time=0;
      int first_job_time= schedule_limit;
      rep(j, J){



        int Jobjnum = best_list[j].size();
        rep(i, Jobjnum){//1-1ジョブごとに初期時間設定をする
          best_list[j][i]=last_job_time;

          last_job_time = max(last_job_time, best_list[j][i]+tau[j][i]+bt);
          first_job_time = min(first_job_time, best_list[j][i]);
        }



        mapping.clear();
        //for greedy



        rep(d, D[j]){
          mapping[make_pair(dependency[j][d].first-1,dependency[j][d].second-1)]=false;//-1して場所にします
        }
        rep(i, TCMB_num[j]){//1-3ジョブ同士の時間制約チェックをする⇐ここをmergeしたい
          tie(tcmb_type,O_1, O_2, alpha) = TCMB.at(j).at(i);
          O_1--; O_2--;
          obj = make_pair(O_1, O_2);
          auto find = mapping.find(obj);

          int inf=schedule_limit;
          int sup=schedule_limit*(-1);

          if(tcmb_type==1){
            inf = -alpha+1;
            sup = alpha-1;
          }
          if(tcmb_type==2){

            inf = tau[j][O_1] -alpha+1;
            sup = tau[j][O_1] +alpha-1;

          }
          if(tcmb_type==3){

            inf = - tau[j][O_2] -alpha+1;
            sup = - tau[j][O_2] +alpha-1;

          }
          if(tcmb_type==4){    

            inf = tau[j][O_1] - tau[j][O_2] -alpha+1;
            sup = tau[j][O_1] - tau[j][O_2] +alpha-1;      
          }
          if (find == mapping.end() ) {
            Dep_and_Order_current.at(j).push_back(make_tuple(O_1, O_2, inf));
            Dep_and_Order.at(j).push_back(make_tuple(O_1, O_2, inf, sup));
            Deped_list.at(j).at(O_1).insert(O_2);
            Depping_list.at(j).at(O_2).push_back(make_tuple(O_1, inf, sup));
          } else {

            Dep_and_Order_current.at(j).push_back(make_tuple(O_1, O_2, max(tau[j][O_1], inf)));
            Dep_and_Order.at(j).push_back(make_tuple(O_1, O_2, max(tau[j][O_1], inf) , min(sup, schedule_limit)));
            Deped_list.at(j).at(O_1).insert(O_2);
            Depping_list.at(j).at(O_2).push_back(make_tuple(O_1, max(tau[j][O_1], inf) , min(sup, schedule_limit)));
            mapping.erase(find);
          }
        }
    

        auto begin = mapping.begin(), end = mapping.end();
        for (auto iter = begin; iter != end; iter++) {
          pair<int,int> MPkey=iter->first;
          Dep_and_Order_current.at(j).push_back(make_tuple(MPkey.first, MPkey.second, tau[j][MPkey.first]));
          Dep_and_Order.at(j).push_back(make_tuple(MPkey.first, MPkey.second, tau[j][MPkey.first] , schedule_limit));
          Depping_list.at(j).at(MPkey.second).push_back(make_tuple(MPkey.first,  tau[j][MPkey.first] , schedule_limit));


            Deped_list.at(j).at(MPkey.first).insert(MPkey.second);
        }
        //ここまででおそらく Dep_and_Order が できたはず
        sort(ALL(Dep_and_Order_current.at(j)));
        sort(ALL(Dep_and_Order.at(j)));
        Dep_and_Order_num[j]=Dep_and_Order.at(j).size();



        rep(i, Dep_and_Order_num[j]){//1-3ジョブ同士の時間制約チェックをする⇐ここをmergeしたい⇐しました

          int O_1, O_2, earliest_dist, latest_dist;
          tie( O_1, O_2, earliest_dist, latest_dist) = Dep_and_Order.at(j).at(i);
          // if(debug_mode) cout << O_1 <<"~"<< O_2 <<":"<<"from "<< earliest_dist<<" to " << latest_dist << endl;//for debug
          Dep_list[j][O_2]=-1;
          best_list[j][O_2]=best_list[j][O_1] + earliest_dist;
          last_job_time = max(last_job_time, best_list[j][O_2]+tau[j][O_2]);
          first_job_time = min(first_job_time, best_list[j][O_2]);


        }    




        if(first_job_time!=0){        //開始時刻が 0 出ない場合の補正
          rep(i, N[j]){
            best_list[j][i]-=first_job_time;
          }

          first_job_time=schedule_limit;
          last_job_time=-1;


          rep(i, N[j]){last_job_time = max(last_job_time, best_list[j][i]+tau[j][i]+bt);
                       first_job_time = min(first_job_time, best_list[j][i]);
                      }
        }
        //いったん無視　1-4ジョブ同士の前後関係チェックをする⇐ここをmergeしたい⇒開始前後の指定を行えば行ける？
        //時間制約で開始同士の制約が決まる(Sbase-制約<=Sdep<=Sbase+制約∩S前+τ<=S後)→UnionFindでグラフ内の代表点のみ決定することで他がすべて決まる→代表点の決定どうしよう→やっぱりUFかな?
        //要するにcombined dependency list を作りたい

      }
    if(debug_mode) cout << "done!\n";

         if(debug_mode){
      rep(j, J){ cout << "J: " << j << endl;
        rep(os, Deped_list[j].size()){
cout << os << " → ";    
            for(auto x : Deped_list[j][os]) {
cout << x << ", " << endl;   // 要素を順に表示
    }
cout << endl;
        }}

      rep(j, J){
          int to, from, time1, time2;
cout << "J: " << j << endl;
        rep(from, Depping_list[j].size()){
            rep(OOO, Depping_list[j][from].size()){
          tie(to, time1, time2) = Depping_list[j][from][OOO];
cout << from << "\t←\t" << to <<"\t"<<time1 << " ~ " << time2 << endl;   
            }
        }}
}
        







      vector<vector<int>> current_list = best_list;
      lli best_score = getScore(N, current_list,tau,Dep_and_Order_num, Dep_and_Order, C, MachineType_num);
      lli currentscore=best_score;



      if(From_greedy){
      if(debug_mode) cout << "From greedy... " << flush;


      //Greedy
      bool Force_finish = false;


      gettimeofday(&time_now, nullptr);
      greedy_start = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

      Greedy(J,  Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish, greedy_time_limit, greedy_start);




      lli greedy_score = getScore(N, greedy_list, tau, Dep_and_Order_num, Dep_and_Order, C, MachineType_num);
      if(greedy_score < best_score){
          best_list = greedy_list;
          current_list = greedy_list;
          currentscore = greedy_score;
          best_score = greedy_score;

          
      }
        if(debug_mode) cout <<  "searched" << endl;
        if(debug_mode) cout << greedy_score << endl;


      
      }


      //初期状態の生成終わり111111111111111111111111111111111111111111111111111111111111111

      rep(j, J){
        rep(o, Dep_list[j].size())
          if(Dep_list[j][o]==-1){
            Dep_list[j][o]=1;//依存してるなら1
          }else{          
            Dep_list[j][o]=0;
          }
      }

      int last_job_time_best = last_job_time;
      int first_job_time_best=first_job_time;





      //生成
      int gen = 0;
      int best_gen = 0;
      //Dep_list: [j][o] j番目のジョブの o 番目のオペレーションが依存するかどうかを保存、１ならスキップ
      //First_step: Set the start time of operations that has no dependence
      //Second_step: Set the interval between the dependence(s) and the operation (random set (acceptance is also randomised))
      //Set the all start time to current_list

      double temp;
      double Temp_S = 0;//Start temperture
      int current_last = 0;
      rep(j, J){
        rep(o, N[j]){
          Temp_S = (double)max(Temp_S, (double)current_list[j][o]);
          current_last = max(current_last, current_list[j][o]);
        }
      }
      temp = Temp_S;

      gettimeofday(&time_now, nullptr);
      time_t annealing_start = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
      vector<int> Temp_L_S(K, 0);
      rep(j, J){
        rep(o, N[j]){
          Temp_L_S[C[j][o]-1]+= tau[j][o] + bt;
        }

      }
      rep(i, K){Temp_L_S[i] /= MachineType_num[i];}
      sort(ALL(Temp_L_S));
      double Temp_L=Temp_L_S[K-1];//Stop temperture
       if(debug_mode) cout << "begin temp: " << Temp_S << " ~ " << "stop_temp:" << Temp_L <<endl;
      gettimeofday(&time_now, nullptr);
      time_t now_the_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
      bool Change_next_S;
      if(debug_mode) cout << now_the_time <<" " << SA_msec<<" " << annealing_start << endl;

      Next_list= current_list;
      int current_last_dep_end;
      int current_last_start; 

      int a, b, c, d; 
      int f;
      Dep_and_Order_next=Dep_and_Order_current;

      
      

      
      while(now_the_time <= SA_msec + annealing_start){//Start generation
        Next_list= current_list;

        current_last_dep_end = min (2 * current_last, schedule_limit);
        current_last_start = min ((int)(2 * current_last), schedule_limit);
        if(2 * current_last<= schedule_limit){
          current_last_dep_end = (int)(schedule_limit- (schedule_limit - 2 * current_last )*(Passed_time/(double)SA_msec));
          current_last_start = (int)(schedule_limit - (schedule_limit - 2 * current_last )*(Passed_time/(double)SA_msec));

        }


        rep(j, J){
          rep(o, N[j]){
            if(Dep_list[j][o]==1) continue;
            Thr = (rand())%10000;
            Th = ((double)Thr)/10000;
            Change_next_S = ((double)Th * (Temp_S-Temp_L) < (double)temp - Temp_L);//ここもともと((double)Th * Temp_S > (double)temp)だったおおミス
            if(Change_next_S){
              double Change_P = (double)(rand()%10000)/10000;
              if(Change_P>=0.5)
                Next_list[j][o]  += ((int)rand())%(int)temp;
              else Next_list[j][o]  -= ((int)rand())%(int)temp;
            }
          }
        }  


        rep(j, J){
          rep(o, Dep_and_Order_num[j]){

            tie(a,b, c,d) = Dep_and_Order.at(j).at(o);
            //変更するかどうかを決定
            Thr = rand()%10000;
            Th = ((double)Thr/10000);
            Change_next_S = ((double)Th * (Temp_S-Temp_L) < (double)temp - (double)Temp_L);
            //下限と上限の差の間の乱数（-4 ~ 5 → 5-(-4)=9, すなわち 0 ~ 9 だから、mod 9+1 で乱数をとる）
            //その乱数から -4 すると、 -4 ~ 5　の乱数生成→それをリストにいれる
            if(Change_next_S){
              if(((min(d-c, int(temp)) + 1) + c) ==0) continue;//ここを変更
              f = (min(d-c, int(temp)) + 1) + c ;
              c = rand()%f;//どう使うか、current_last_dep_end - 前回の仕事の開始時刻→前回の仕事の開始時刻から終了時刻までの幅→これとd を比較

            }
            else{c = std::get<2>(Dep_and_Order_current[j][o]);

                }

            Dep_and_Order_next.at(j).at(o)=make_tuple(a, b, c);

          }
          sort(ALL(Dep_and_Order_next.at(j)));
          rep(o, Dep_and_Order_num[j]){
            tie(a,b, c) = Dep_and_Order_next.at(j).at(o);
            Next_list[j][b]  = Next_list[j][a] + c;//ここで、Next_list 完成
          }
        }

        first_job_time = current_last_dep_end;

        rep(j, J){

          rep(o, N[j]){last_job_time = max(last_job_time, Next_list[j][o]+tau[j][o]);
                       first_job_time = min(first_job_time, Next_list[j][o]);

                      }
        }


        if(first_job_time!=0){    
          rep(j, J){    //開始時刻が 0 出ない場合の補正
            rep(o, N[j]){
              Next_list[j][o]-=first_job_time;

            }
          }
        }


        lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);


        Thr = (int)rand()%10000;
        Th = ((double)Thr/10000);
        Force_next = (Th < (std::exp((currentscore - nextscore)/(temp-Temp_L+1))));


        if(Force_next){

          currentscore=nextscore;
          current_list = Next_list;
          Dep_and_Order_current = Dep_and_Order_next;
          if(currentscore < best_score){


            best_score=  currentscore;
            best_list = current_list;
            last_job_time_best = last_job_time;
            first_job_time_best=first_job_time;
            best_gen = gen;

          }
        }

        gen ++;

        gettimeofday(&time_now, nullptr);
        now_the_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
        Passed_time = (double)((double)now_the_time-(double)annealing_start);
        if(Passed_time > SA_msec||Passed_time < 0){break;}
        temp = Temp_S - (Temp_S - Temp_L)*(Passed_time/(double)SA_msec);
        //方針１0 - bestscore という時間内での焼きなまし法、
      }


      //_________________________________最終調整

      Local_fitting_J1_updated =true;
      Local_fitting_J2_updated =true;
      Local_fitting_J1_start:
      Local_fitting_J1_updated = false;

      Local_fitting_J1 = true;

      last_time_start = 0;
if(SA_naive) goto SA_naive_skip;
if(command_line.count("-fs") > 0 &&best_score >= schedule_limit){
  goto SA_naive_skip;
}


    if(debug_mode) cout << "Last_1" << endl;
      rep(j, J){

        rep(o, N[j]){
          last_time_start = max(last_time_start, best_list[j][o]);
        }
      }
      // kokomousukosihayakukakeru
       if(debug_mode) cout << "Last_modify_could be modified";
      while(Local_fitting_J1){//Start generation
        Local_fitting_J1 = false;



        rep(time_criteria, last_time_start){//引き算する基準時間

          Next_list= best_list;                 
          rep(j, J){

            rep(o, N[j]){
              if(Next_list[j][o] > time_criteria){
                Next_list[j][o]--;
              }
            }
          }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore < best_score){
            best_score=  nextscore;
            best_list = Next_list;
            last_time_start --;
            Local_fitting_J1 = true;
            Local_fitting_J1_updated = true;
          }
        }


      }
      if(Local_fitting_J1_updated == false && Local_fitting_J2_updated == false) goto Endfitting;

    if(debug_mode) cout << "Last_2" << endl;
      Local_fitting_J2_updated =false;
      


      {
      Local_fitting_J2_start:
      
          
      Local_fitting_J2 = true;
          
// 1 JOB ごとに最終調整
      while(Local_fitting_J2){//Start generation
        Local_fitting_J2 = false;


              
          rep(j, J){
        rep(time_criteria, last_time_start){//引き算する基準時間

          Next_list= best_list;   

            rep(o, N[j]){
              if(Next_list[j][o] > time_criteria){
                Next_list[j][o]--;
              }
            }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore < best_score){
            best_score=  nextscore;
            best_list = Next_list;
            Local_fitting_J2 = true;
            Local_fitting_J2_updated = true;
          }
          }
        }
      }
      if(Local_fitting_J2_updated) goto Local_fitting_J1_start;


      }
      Endfitting:
    if(debug_mode) cout << "Endfitting" << endl;


// 1 JOB ごとに最終調整
if(Align_start){
cout << "Aligning ..." << endl;
  if(debug_mode) cout << " ..." << flush;
      Local_fitting_J1_updated =true;
      Local_fitting_J2_updated =true;
    
      bool Align_start_J1 = true;
      bool Aligned = false;
      Aligning:

vector<vector<int>> KEEP_BEST = best_list;
      Aligned = false;
      Align_start_J1 = true;

      while(Align_start_J1){//Start generation
        KEEP_BEST = best_list;
        Align_start_J1 = false;


              
          rep(j, J){
        rep(time_criteria, last_time_start + 1){//引き算する基準時間

          Next_list= best_list;   

            rep(o, N[j]){
              if(Next_list[j][o] <= last_time_start - time_criteria){
                Next_list[j][o] = max(Next_list[j][o]-1, 0);
              }
            }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore <= best_score){
            
            best_score=  nextscore;
            best_list = Next_list;

          }
          }
        }
            if(KEEP_BEST != best_list)
            {Aligned = true;
            Align_start_J1 = true;
             }
      }


      if(Aligned){
      Local_fitting_J1_start_after_aligned:
      Local_fitting_J1_updated =false;
      Local_fitting_J2_updated =false;
      Local_fitting_J1 = true;
      Local_fitting_J2 = true;

    if(debug_mode) cout << "Last_1" << endl;
    
      while(Local_fitting_J1){//Start generation
        Local_fitting_J1 = false;



        rep(time_criteria, last_time_start){//引き算する基準時間

          Next_list= best_list;                 
          rep(j, J){

            rep(o, N[j]){
              if(Next_list[j][o] > time_criteria){
                  if(Next_list[j][o] > 0)Next_list[j][o]--;
              }
            }
          }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore < best_score){
            best_score=  nextscore;
            best_list = Next_list;
            last_time_start --;
            Local_fitting_J1 = true;
            Local_fitting_J1_updated = true;
          }
        }


      }



    if(debug_mode) cout << "Last_2" << endl;
      while(Local_fitting_J2){//Start generation
        Local_fitting_J2 = false;


              
          rep(j, J){
        rep(time_criteria, last_time_start){//引き算する基準時間

          Next_list= best_list;   

            rep(o, N[j]){
              if(Next_list[j][o] > time_criteria){
                  if(Next_list[j][o] > 0)Next_list[j][o]--;
              }
            }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore < best_score){
            best_score=  nextscore;
            best_list = Next_list;
            Local_fitting_J2 = true;
            Local_fitting_J2_updated =true;
          }
          }
        }
      }
      if(Local_fitting_J2_updated) goto Local_fitting_J1_start_after_aligned;


      if(Local_fitting_J1_updated||Local_fitting_J2_updated) goto Aligning;
      }


      }
    if(debug_mode) cout << endl;

      //Strict_adjustment

if(Strict_adjustment){
      vector<vector<int>> KEEP_BEST = best_list;
    //rough -> med -> fine
  if(debug_mode) cout << "Strict_adjustment" << flush;
      Local_fitting_J1_updated =true;
      Local_fitting_J2_updated =true;
      Local_fitting_J3_updated =true;
    
      bool Align_start_J1 = true;
      bool Aligned = false;
      Rough_adjustment:

      Local_fitting_J1_updated =false;
      
      Local_fitting_J1 = true;
      

    if(debug_mode) cout << "Last_1" << endl;
    
      while(Local_fitting_J1){//Start generation
        KEEP_BEST = best_list;
        Local_fitting_J1 = false;
        int presence = 0;



        rep(time_criteria, last_time_start){//引き算する基準時間

          Next_list= best_list;                 
          rep(j, J){

            rep(o, N[j]){
              if(Next_list[j][o] > time_criteria){
                  if(Next_list[j][o] > 0)Next_list[j][o]--;
                presence = max(time_criteria+1, presence);
              }
            }
          }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore <= best_score){
            best_score=  nextscore;
            best_list = Next_list;
          }
        }

        last_time_start = presence;
        if(KEEP_BEST != best_list) 
        {
            
      Local_fitting_J1 = true;
      Local_fitting_J1_updated =true;
        }


      }



    if(debug_mode) cout << "Last_2" << endl;
      Local_fitting_J2 = true;
      Local_fitting_J2_updated =false;
      while(Local_fitting_J2){//Start generation
        KEEP_BEST = best_list;
        Local_fitting_J2 = false;


              
          rep(j, J){
        rep(time_criteria, last_time_start){//引き算する基準時間

          Next_list= best_list;   

            rep(o, N[j]){
              if(Next_list[j][o] > time_criteria){
                  if(Next_list[j][o] > 0)Next_list[j][o]--;
              }
            }
          //scorecheck

          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

          if(nextscore <= best_score){
            best_score=  nextscore;
            best_list = Next_list;
          }
          }
        }


        if(KEEP_BEST != best_list) 
        {
            
            Local_fitting_J2 = true;
            Local_fitting_J2_updated =true;
        }


      }
      if(Local_fitting_J2_updated) goto Rough_adjustment;


    if(debug_mode) cout << "Last_fine" << endl;
      Local_fitting_J3 = true;
      Local_fitting_J3_updated =false;

      while(Local_fitting_J3){//Start generation
        KEEP_BEST = best_list;
        Local_fitting_J3 = false;


              
          rep(j, J){

          Next_list= best_list;   

            rep(o, N[j]){
                if(Next_list[j][o] != 0){
                  Next_list[j][o] --;
          lli nextscore = getScore(N, Next_list,tau,  Dep_and_Order_num, Dep_and_Order, C, MachineType_num);
          //scorecheck
          if(nextscore <= best_score){
            best_score=  nextscore;
            best_list = Next_list;
            Local_fitting_J3 = true;
            Local_fitting_J3_updated =true;
          }
          else    Next_list[j][o] ++;
          }

              
            }
          
        }


      }
      if(Local_fitting_J3_updated) goto Rough_adjustment;
      


      }
    if(debug_mode) cout << endl;
      SA_naive_skip:



      //_________________________________最終調整

      if(With_greedy){
          adopt_greedy = false;
          

      gettimeofday(&time_now, nullptr);
      greedy_start = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

      //Greedy
      Greedy(J, Deped_list, Depping_list, Strict_list_for_greedy,   greedy_list,  N,  tau, Dep_and_Order_num,   Dep_and_Order,   C,  MachineType_num, Force_finish, greedy_time_limit, greedy_start);
    if(debug_mode) cout << "Greedy done!" << endl;
      lli greedy_score = getScore(N, greedy_list, tau, Dep_and_Order_num, Dep_and_Order, C, MachineType_num);

      if(greedy_score < best_score){
          best_list = greedy_list;
          best_score = greedy_score;
          adopt_greedy = true;
      }

      }
      if(From_greedy){
          if(best_gen == 0){
              
          adopt_greedy = true;
          }
      }








      C_ans = C;
      Used_num_of_machine_types.resize(K);
      Used_machines.resize(M, false);
      rep(Fill0, K)Used_num_of_machine_types[Fill0] = 0;
      rep(Fillf, M)Used_machines[Fillf] = false;




      Booking_List_Generator(C_ans,N,best_list,tau,  T,  C);


      rep(Job, J){
          rep(Operation, N[Job]){
              Used_machines[C_ans[Job][Operation]-1] = true;

          }

      }
      rep(i, M){
          if(Used_machines[i]){
              Used_num_of_machine_types[T[i]]++;
          }

      }

      if(best_score >= schedule_limit){
        FAIL = true;

      }

      rep(i, K){
          Result[Loops * ppp + Loop][K+i] = Used_num_of_machine_types[i];

      }


      gettimeofday(&time_now, nullptr);
      now_the_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

     if(debug_mode) cout << "last step: " << gen << endl;
     if(debug_mode) cout << "best step: " << best_gen << endl;


      if(FAIL)
        fileName_time = Output_directory + test_time_ + to_string(best_score) +"_"+to_string(ppp)+"_Loop" +to_string(Loop)+ "_FAILED" +tsvtxt;
      else if(adopt_greedy)
        fileName_time = Output_directory + test_time_ + to_string(best_score) +"_"+to_string(ppp)+"_Loop" +to_string(Loop)+ "_GREED" +tsvtxt;
      else
        fileName_time = Output_directory + test_time_ + to_string(best_score) +"_" +to_string(ppp)+"_Loop" +to_string(Loop)+  tsvtxt;

      ofstream ofs(fileName_time);
      if (!ofs)
      {
        std::cout << "ファイルが開けませんでした。" << std::endl;
        std::cin.get();
        return 0;
      }
      ofs << "Job_ID" <<"\t" << "Operation_ID" <<"\t" << "Start" <<"\t" << "End" <<"\t" <<"Machine_ID" <<  std::endl;
      rep(j, J){
        rep(o, N[j]){
          if(j==J-1&&o==N[J-1]-1)ofs << j+1 <<"\t" << o+1 <<"\t" << best_list[j][o] <<"\t" << best_list[j][o]+tau[j][o] <<"\t" <<C_ans[j][o];
          else
            ofs << j+1 <<"\t" << o+1 <<"\t" << best_list[j][o] <<"\t" << best_list[j][o]+tau[j][o] <<"\t" <<C_ans[j][o] <<  std::endl;


        }

      }
      ofs.close();

      std::cout << fileName_time << "に書き込みました。" << std::endl;

      if(FAIL)
        fileName_dep = Output_directory + test_dep_ +to_string(best_score)+"_"+ to_string(ppp) +"_Loop" +to_string(Loop)+ "_FAILED" +tsvtxt;
      else if(adopt_greedy)
        fileName_dep = Output_directory + test_dep_ +to_string(best_score)+"_"+ to_string(ppp) +"_Loop" +to_string(Loop)+ "_GREED" +tsvtxt;
      else
        fileName_dep = Output_directory + test_dep_ +to_string(best_score) +"_"+ to_string(ppp) +"_Loop" +to_string(Loop)+tsvtxt;


      ofstream ofs2(fileName_dep);
      if (!ofs2)
      {
        std::cout << "ファイルが開けませんでした。" << std::endl;
        std::cin.get();
        return 0;
      }
      ofs2 << "Job_ID" <<"\t" << "Operation_ID_1" <<"\t" << "Operation_ID_2"<<std::endl;
      rep(j, J){
        int a, b;
        rep(d, D[j]){
          if(j==J-1&&d==D[j]-1)ofs2 << j+1 <<"\t" << dependency[j][d].first <<"\t" << dependency[j][d].second;
          else
            ofs2 << j+1 <<"\t" << dependency[j][d].first <<"\t" << dependency[j][d].second << std::endl;


        }

      }

      ofs2.close();

      std::cout << fileName_dep << "に書き込みました。" << std::endl;


      if(FAIL)
        fileName_mac = Output_directory + test_machines_ + to_string(best_score) +"_"+ to_string(ppp)+"_Loop" +to_string(Loop)+ "_FAILED" +tsvtxt;
      else if(adopt_greedy)
        fileName_mac = Output_directory + test_machines_ + to_string(best_score) +"_"+ to_string(ppp)+"_Loop" +to_string(Loop)+ "_GREED" +tsvtxt;
      else
        fileName_mac = Output_directory + test_machines_ +to_string(best_score)+"_"+ to_string(ppp)  +"_Loop" +to_string(Loop)+tsvtxt;

      MAC_COUNT = 0;
      ofstream ofs3(fileName_mac);
      if (!ofs3)
      {
        std::cout << "ファイルが開けませんでした。" << std::endl;
        std::cin.get();
        return 0;
      }
      ofs3 << "Machine_ID" <<"\t" << "Machine_type" <<"\t" << "Machine_name"<<std::endl;

      ifstream inp_file(Machine_Source);
      if (!inp_file.is_open()) {
        cout << "Could not open the file - '"
          << Machine_Source << "'" << endl;
      }
      string F;
      rep(j, 3){
        inp_file >> F;
      }
      while (!inp_file.eof())
      {
        inp_file >> F;

        if(isNumber(F)){
          MAC_COUNT ++;
          if(MAC_COUNT != 1) ofs3 << endl;
          ofs3 << std::stoi(F) << "\t";
          int Type;
          inp_file >> Type;
          //Result[Loops * ppp + Loop][Type - 2] ++;
          ofs3 << Type << "\t";
        }
        else ofs3 << F << "\t";

      }


      //ofs3 << endl;
      rep(oppp, K){
        rep(opppp, ADD_bit[oppp]){
          MAC_COUNT ++;
          ofs3 << endl << MAC_COUNT << "\t" << oppp+1 << "\t" << "ADD_Machine_Type:" << oppp+1;
        }

      }


      inp_file.close();





      ofs3.close();
      std::cout << fileName_mac << "に書き込みました。" << std::endl;






      Result[Loops * ppp + Loop][2 * K ] = best_score;      
      ofstream writing_file;
      fileName_res = Output_directory + test_result_ +tsvtxt;
      gettimeofday(&time_now, nullptr);
      time_t Scheduling_end_sec = time_now.tv_sec;
      time_t Scheduling_end_msec = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
      string Result_statas;


      if(FAIL)
Result_statas = "FAILED";
      else if(adopt_greedy)
Result_statas = "GREED";
      else
Result_statas = "SA";
      if(!checkFileExistence(fileName_res))  {
        writing_file.open(fileName_res, std::ios::out);
        writing_file << "DATE(GMT): YYYY/ MM/ DD/ hh/ mm/ ss\t";
        writing_file << "ID\t";
        writing_file << "Loop_number" <<"\t";
        writing_file << "Set_number" <<"\t";
        rep(i, K) writing_file <<"Machine" <<  to_string(i+1) <<"\t";
        rep(i, K) writing_file <<"Used_Machine" <<  to_string(i+1) <<"\t";
        writing_file<<  "Result_Time" << 
        "\t" << "Annealing_time_msec"<< 
        "\t" << "Scheduling_time_sec"<<  
        "\tScheduling_time_msec"<<
        "\tBest_step"<<
        "\tLast_step"<<
        "\tResult_statas"<< endl;


        writing_file.close();
      }
      writing_file.open(fileName_res, std::ios::app);

      int Place = Loops * ppp + Loop;
      writing_file << getDatetimeStr() << "\t";
      writing_file << Experimental_ID << "\t";
      writing_file << Loop << "\t";
      writing_file << ppp << "\t";
      rep(j, Result[Place].size()){
        if(Result[Place].size()-1 == j) {
          writing_file << Result[Place][j] << "\t";
          writing_file << SA_msec << 
          "\t" << Scheduling_end_sec - Scheduling_start_sec << 
          "\t" << Scheduling_end_msec - Scheduling_start_msec <<
          "\t" << best_gen <<
          "\t" << gen <<
          "\t" << Result_statas << endl;
        }
        else{
          writing_file << Result[Place][j] <<"\t";
        }
      }


      writing_file.close();


      std::cout << fileName_res << "に書き込みました。" << std::endl;

      cout <<"______" << endl;

    }
     if(debug_mode) cout << "Loop " << Loop << " finished" << endl;

  }




  rep(i, Result.size()){


    rep(j, Result[i].size()){

      cout << Result[i][j] << " ";
    }
    cout <<endl;


  } 
  return 0;
}
