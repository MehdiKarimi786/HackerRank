﻿#include <string>
#include <vector>
#include <iostream>

#include "serry.hpp"

using namespace std;

auto tests = vector<pair<string, int>>{
    {"0", 0},
    {"2", 1},
    {"3", 0},
    {"2222222", 7},
    {"24256", 4},
    {"65536", 1},
    {"023223", 4},
    {"33579",0},
    {"9303535670983768199031344740966458039726609416797671171603074549512182887851493418575245449136173639177760276560207077549242900846267596823817051317718446589520242536874132581700120107002038199303870846751188192899823151552628349788604516295066307994130118526061826166445047808309163001841380667575628151274558987543914186376514799892578820116121531383164833962895501326553806236997089282520174174189206292883439012459432693877366459895758465185873923518437208883287869410049671804351768330228241833181048771841834309240249132277574952747489997467168763400466618303709392785810954982875161446396373040800947562126272731545618170968107390172263733095197200113358841034017182951507037254979515982202834948083154776267844089139019063040156654448338365040715366458968162887836583628774290327941701420576894069006881693378223441337877537377325813845730080900918242835443359855685076558915384842574884883772410178635875682021801984576460752303423488223007451985306231415357182726483615059804162147483648324", 464},
    {"1844674407370955161674106937111882365071085430405560261026092790186009960985252853765064402969559046455624695217271474139797930007529685824264482073058782076648391351619055042102986574113383200344578589757929931868733441327844982041917746723970516381171568323982794317579807998610345501008899652130606847906255663073214172223323715616252538366448344131768098523799946916468379859578177088483047579320327229475734293036792189718636043847330179466014340038463189508943008496205724668159759037237747788792245498535675607031239995639976648680825923975906526582032462837994195753268665938105581321030972818840265816397736281374726739986666787659948666753771754907668409286105635143120275902562304104187725513747723032497684230019653080386848786186065006191528308813081840900501117522378138618035792858279853022394381967012525845615079380677438317669219470236837179906477475985598217372094136390078377123228155963917938085569707674435584356811923176489970264571492362373784095686656302231454903657293676544",407},
    {"3599131035634557106248430806148785487095757694641533306480604458089470064537190296255232548883112685719936728506816716098566612844395439751206812144692131084107776748288838313422294120286634350736906063837462003712327683518437208883214599809976391024699651749138240932239658331223195391777853416057277680535067767636818920996215588479291655390635502103394203855108401401594408516223111085402406382957952847840265197415189132083237942751801283770916395740367646853645359508575237100024449461127712974320414228483777893186295716170956896613438075431458617383797273299683607473183242660874966430881286287978557239010613404844164548064449061590400787554429434126966526074691393572716836677018717424520370585614678391142336457674309253729933356421098015930676999191920568572076306406966302771648118739904804393949593610576895500643977583230644928524336637254474927428499508554380724390492659780981533203027367035444557561459392400373732868096153249554086588885835834702715030918361873912218360217626214421",447},
    {"4074071952668972172536891376818756322102936787331872501272280898708762599526673412366794752198929294563914656862152899258728336040182460318939086976185590757263798805013350213222449466080294620906812100504203929438007026269820242367982812611218579445021306373434063280212248608997919534285203227867870273006861350241993509231020378633583321354429739827226328072917139296674479506920917608079723773850137277813577744384302231454903657293676544256842573317791675158251459125006282497575185684725286108314497220152937221537149517268873841679123595067527776180577036768526965341006048004287830598236369626719905816818583896077331927945445179262365807411222615642429163319418666208009509357002591793880007922663956559376545533132812089258196146291747061764737909217226284592129454005317695786306414847157887607235876580890048073383718525179682644130390862886009920340365823993443542615096479466077536850101260800794845507876128862152909196873774550858346555156605434420449361736499227487790694483886081616",426},
    {"1645504557321206042154969182557350504982735865633579863348609024230584300921369395284162174424773976115855838126082058648805436845170781751972494449099714468753293153818664580441415219631727501698851483408310916002940861810045036330430093599283578738055113571066620126149163476340692027772502016247588007857076054979824844814678391142336457674309253729933356421098015930676999191920568572076306406966302771648118739904804393949593647223664828696452136961913626293225524389432754063047515470516496790086666391106802949459500143092402439693129612815969613157715855361376531696085087696768388509782313038395616185864209427064795672119239955603669920409190451644997561738171793118383440060237486594115856584470256613187130812952440336823892592907065602756628718063439454949867525368709123923188584616675477397368389504791510063972152790021570565127136238463529799405291429847247475681913733121367031702989382452732813891948513353345730894308257772766106629006220624499609952014695735639408646412851221024",433},
    {"1067993517960455041197510853084776057301352261178326384973520803911109862890320275011481043468288850705917302346158658436518579420528647729075046034516689390703781863974688597854659412869997314470502903038284579120849072387533163845155924927232063004354354730157322085975311485817346934161497393961629646848994646472819573284310764496293641680200912301594695434880927953786318994025066751066112121290475960992885558514022536133012129444220087241922745238440470785230678623194244599875689738006089881853960713365094232154690946469874331584943362592276500348045001628898871114475439968628501973671812009099121163503660454379522632807291713929667447950692091760807972377385013727781357774438439785858912782931372430579851745667208036492063787817395237118151452759761002670042644482361183241434822606848167597599124282463744675312477573076593492072757404917221544518046522050375919337210023428727086292846125398227331075635671923535149332124330420612576051246116860184273879045497558138881171798691841281",458},
    {"4784065733063810973581885157618788676291241975216665977767007373648750357731006099232824032039924032894289638403441329240212719241920971274455782595989040464660523567661989180298099889009174801022976193428131138340667952988161652639921975621497379788270081927599571011707410703048211621988186014478090778364562973026099288212118978030062558395760643995838144404470056168444454135252871358205622611163073099720908320475825689299993753991811921269723084578471835400477306173408869489005192051425281309356243158456748005275878731039660886656818417225915793316547238453518561869821953308036930361662860354673651024028403686902618354157221331411035750415095849699286165408966218323953075563667684881665761713504825200982496649568595408344506900710635070165873500123055379599082338519699170530245737005730784174597581307904294840814439182918143871451639708507102884470345034408466891117206689387686886629069228650404504591214177216799278425382794576924212874424418862050893179378410109009924294967296161024",445},
    {"7128813465034680029126830633906705195145669542575874891529988309217617448763558416415488767683392812154128608708741261422951101127149121872269783312065959669143741678687913984504490925385318481396660557615408619138815804702853969052225614207863286087131491260220188203516498064357920795340463170531135173745610911728535816886780766154248685679077711122529046801935528113748607321948544054748732127521996846810889129351334120652842535295865117307932921825928971026432186877567697805116155542388969483932667626639656901014756523725533154310848867425751282188751559532534933189000134426923445809345387537940408429007655821893150801701861796452355394526914420890664966156563468186637376918600015647439657043709261010226041866920844413394026796439158033479102325768068876035623485445415370496329716522614090203404460358274291162843391748379842930887932241807862544999950011922147613471467208908991351228465152144740111546645244279463731260859884816587480832050705049321980009891412049921125899906842624864",483},
    {"1684996666696914987166688442938726917102321526408785780068975640576573374653997517877902705223825521735199141247292070280934397209846730719022121202017504638277531421638656370534685559411825355427152027801305130463950930049804926264268825322014847795232768948568795032094272909893509191171341133987714380927500611236528192824358010355712327682074757784440496479256203931845580575506223116121218449997828664845326405706454073199853524473551897144098943305650394591197575537705887653943437417056981843530590901700754761842688463168356949264781694283940034751631413079938662562256157830336031652518559744439347050248359021758841651141209165905243859209171546201245661387874763737449987335843817002333091518546963929054774914375807231981865204004737810631363657728207475778444049647925620393184558057550622311612121844999782866484532640570645407319985352447355189714409894330565039459119757553770588765394343741705698184353059090170075476184268810384593717069655257060992658440192655361152921504606846976",435},
    {"1524291284333980581729295522359944485228807686848130444755447734192076044345588681699368214386470689042884243711624327585667956874652483059712111387710391166875455106728654792268674151086602748045180156067331525272636930600256492011995053012689908259511074082209733610955111705029254215364251030619830370963729498656007882670709145612842128665889583757912572956250314124878759284236264305415724861007646861076857475863443692083956179753376388809028851838426348267050302400214391529911818481335995290840929194803866596397272258963118290370563450873173395281893717377931138512726225554486085193277581262111899648143965241425382284249937232245951419483830307785661332259224178323578802581487611850209301955324507428797469140272668643942664513775817590048272485787685243364311042338402619729444669125895732346052831449492068761614536774485912137810986476157760090687072827213746361205629803983612785762267958466523821014275271311215250432125323558670692032572293121180591620717411303424439804651110412816",484},
};

#include <fstream>
//#define AAA
#ifdef AAA
    #define SERRY SerryA
#else
    #define SERRY SerryB
#endif
//#define SERRY SerryBoolean
void log(int idx, int i)
{
#ifdef AAA
    std::ofstream out("outputA.txt", std::ios_base::app);
#else
    std::ofstream out("outputB.txt", std::ios_base::app);
#endif

    out << idx << "," << i << endl;
    out.close();
}

int twoTwo(string a)
{
    auto sz = a.size();
    vector<Contianer> serries;
    for (auto idx(0u); idx < sz; ++idx)
    {
        serries.push_back(getValues(a[idx] - '0'));
    }

    auto getCount = [&](int idx) {
        int result = 0;
        if (a[idx] == '0')
        {
            return result;
        }

        SERRY curVal(a[idx] - '0');
        if (curVal.isPowOf2())
        {
            ++result;
        }

        long long v;
        for (auto i(idx + 1); i < sz; ++i)
        {
#ifdef DEBUG
            v = curVal.value();
#endif
            if (!curVal.action(serries[i]))
            {
                break;
            }
            if(curVal.action(serries[i]) && curVal.isPowOf2())
            {
#ifdef DEBUG
                log(idx, i);
#endif
                ++result;
            }
#ifdef DEBUG
            v = curVal.value();
#endif
        }
        return result;
    };

#define SERIAL
#ifdef SERIAL
    int sum = 0;
    for (int idx(sz-1); idx >= 0; --idx)
    {
        auto v = a[idx] - '0';
        if (v == 2 || v == 4 || v == 6 || v == 8)
            break;
        --sz;
    }
    for (auto idx(0u); idx < sz; ++idx)
    {
        sum += getCount(idx);
    }
    return sum;
#endif  // SERIAL

#ifdef EXECUTION_POLICY
    vector<int> indices(sz, 0);
    for (auto i(0); i < sz; ++i)
    {
        indices[i] = i;
    }

    atomic_int sum = 0;
    for_each(execution::par, indices.cbegin(), indices.cend(),
        [&](int idx)
        {
            sum += getCount(idx);
        });
    return sum;
#endif  // EXECUTION_POLICY

#ifdef THREAD
    const int grainsize = (sz + 8) / 8;

    auto getCountP = [&](promise<int> p, int idx) {
        auto sum = 0;
        for (auto i(idx); i < sz && (i - idx) < grainsize; ++i)
        {
            sum += getCount(i);
        }
        p.set_value(sum);
    };

    const int COUNT = 8;
    promise<int> promises[COUNT];
    std::future<int> futures[COUNT];
    vector<thread> threads(COUNT);
    
    int work_iter = 0;
    auto pp = 0;
    for (auto it = begin(threads); it != end(threads); ++it) {
        futures[pp] = promises[pp].get_future();
        *it = thread(getCountP, move(promises[pp]), work_iter);
        work_iter += grainsize;
        ++pp;
    }

    int sum = 0;
    for (int i = 0; i < COUNT; ++i)
    {
        sum += futures[i].get();
    }

    for (auto&& i : threads) {
        i.join();
    }
    
    return sum;
#endif  //THREAD

}

int main()
{
    for (auto test : tests)
    {
        auto result = twoTwo(test.first);
        cout << "[" << test.first << "," << test.second << "]: " << result << endl;
    }
    return 0;
}
