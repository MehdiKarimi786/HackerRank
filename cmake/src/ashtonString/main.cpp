#include <string>
#include <string_view>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>

#define WITH_RANGES

#ifdef WITH_RANGES
#   include <ranges>
#endif
using namespace std; 

char ashtonString(string s, int k) {
    auto sz = s.size();
    auto data = s.data();
    set<int> minPositions;
    
#ifdef WITH_RANGES
    auto getMinPostion = [&](){
        auto freeIndices = views::iota(0, (int)sz) | views::filter([&](int i) { return minPositions.find(i) == minPositions.end(); });
        auto minIt = ranges::min_element(freeIndices, [&](auto L, auto R) {
            auto strL = string_view(data + L, sz - L);
            auto strR = string_view(data + R, sz - R);
            return strL < strR;
            });
        minPositions.insert(*minIt);
        return *minIt;
    };
#else
    std::vector<int> freeIndices(sz, 0);
    for (auto i(0); i <sz;++i)
    {
        freeIndices[i] = i;
    }

    auto getMinPostion = [&]() {
        auto minIt = std::min_element(freeIndices.begin(), freeIndices.end(), [&](auto L, auto R) {
            auto strL = string_view(data + L, sz - L);
            auto strR = string_view(data + R, sz - R);
            return strL < strR;
            });
        auto mn = *minIt;
        minPositions.insert(mn);
        auto it = std::remove(freeIndices.begin(), freeIndices.end(), mn);
        freeIndices.erase(it, freeIndices.end());
        return mn;
    };
#endif

    auto reachBefore = [&](int mn, int length) {
        for(auto minPos: minPositions)
        {
            auto lengthMin = sz - minPos;
            if (lengthMin < length)
            {
                continue;
            }

            if (minPos == mn)
            {
                continue;
            }

            if (memcmp(data + minPos, data + mn, length) == 0)
            {
                return true;
            }
        }
        return false;
    };

    auto ch = s[0];
    for (auto idx(0ul); idx < sz; ++idx)
    {
        auto mn = getMinPostion();
        auto length = 1;
        for (auto j(mn); j < sz; ++j, ++length)
        {
            auto found = reachBefore(mn, length);
            if (found)
            {
                continue;
            }

            if (length >= k)
            {
                return data[mn + k - 1];
            }
            k -= length;
        }
    }

    return ch;
}

int main()
{
    char ch;
    string s;
    
    s = "aaasssaaa";
    std::cout << ashtonString(s, 1) << std::endl;
    std::cout << ashtonString(s, 2) << std::endl;
    std::cout << ashtonString(s, 3) << std::endl;
    std::cout << ashtonString(s, 4) << std::endl;
    
    
    if (1)
    {
        s = "dbac";
        string allSubs = "aacbbabaccddbdbadbac";
        for (auto i(1); i <= allSubs.size(); i++)
        {
            ch = ashtonString(s, i);
            cout << ch;
        }
    }
    
    s = "nvzjkcahjwlhmdiuobjdwbanmvrtadopapbktdtezellktgywrdstdhhayaadqrdhspavjgxprk";
    if (1)
    {
        ch = ashtonString(s, 1);
        ch = ashtonString(s, 2);
        ch = ashtonString(s, 3);
    }
    ch = ashtonString(s, 2071); // b

    s = "szkkcedhlkhjnjofbrnvhntsushncjavkmfn";
    ch = ashtonString(s, 465); // d

    s = "wcweojncpqwcofrhxnzgbdrd";
    ch = ashtonString(s, 251); // d
    
    s = "pfpgrnlorzzhdoxzsnemubzvkcbbfb";
    ch = ashtonString(s, 77); // o

    s = "judaioobpoiteiszvzlscmpmpqqwuvtdqzdapudfimaowsnttalwndievaapwusmtyoksrpcfpqbkgvfiibvlkbjkcy";
    ch = ashtonString(s, 2473); // w

    s = "toefpqnvnqducsamtcmqiqyjazyhizgcnmjefzzurcpturfqtsgbkgnmfntombsbodftcgnukenfvvxrpdszlfoqvhghjylzcssgzhajoqqjnnacsvddcrwxbchkdslhkfnlmqvainmvbozvjeboyxlzbslemwnxbdiptfsdtgywuztgduubthavcnbojplmuwepdwswcquyroeuiayciaxkoaazrmolksanqsmskgsbuzyfzyhjygvogwqakelwymjogxiteauabufbsomtuijdgbdqfqmfexvlvdebgadhxjkrxxmuhwznxedfurmbqimllsotusdrdnlcmxwttxgtblayemzvuniihxbdrgvuugwiftcbskuwyuucgvxdkglrfouwvrqrzmzfhbgaocwmysogpojzuusbloxginxhbwolawloajcydqfsgqtallbyzaejpdtrbhcdgptiavhfooxueqxrdapebvoqahjcqlhwcbfcyohocglgyiabkpgnnwgndprtdzsfczkaprrraczykzawrikeeqtjflcikwqmvaokrfdthfstguqxccdisxrzivkusaipayctgfnnmhjtbbsfdynxvgwfdgbvilnkjpgpwtfjaocdruiwsxtpdrwgazeikrtugzjcuqlwepzxmjtegowlgutguxqeqlywmkbiaoghefgqqcwwqujypdejaxpriqpxcsfcjmlntsfmudkmaulpxrazorszhkyjcfolrbanvhbrnlenfreeieezvyyfiyqndgaxkdkfmlzzyfmewqiewodundzvbrifylcioopabocztofrhowfcbbphcniuxpuiteyjvzmkbmfrrxahvimyjbfmqqiohcydixmdwzpznvsgutordapmdxawnikumkxuhmyflneakzgguuxxwpjcmmyzuivhuvbcjbhupowzncghzfgvurxgdxhyhfhcckelmnhbbdcqfkzeqfcmxbtazcaghhijsowecdhfglymwxsofwaeavgzahhhqstkgrqkvzqblqphnkxugxygvfhvoreglxttpjemlwnwmdfbncyvbwcwdlrtewcpvvjmgnyrlmnzruaewybyxdwcroyvkanfvwsdlsvwekvxfxddyfdviazboxzaxmfuizytstryfoyklbolgtgrtishfthffpbpqzkkublarjmdkcorwxirhcbmwktbzvtswdcthpvyyhdlmuckrmbyocmmniqofjgbmluvcpubzxoltqvkeykubzhqjxeognpuamqcdmdclrpfjmrplekofreoqieyxvsxhkanyfrmzhrkvkagqmwvfalvlptilnisaihapzmoidzdqbmgnicslnpyfiiqxqkzasaqrmebsfeiisqvdupoiempoufmnpnnjndaakduricacuvfpntzrhogeturkjeuxkuzwatnkxnmrlsgynfrxtzbotszfytcipbepuuzrhmkugrstykqsjtiflhkmaowqpbhlxheevozbhtwgfpapiiwwqiisweklhsygbclysmbahxgnogezoavgifzgljtvdrbevocncdpkdvxtdbsudnanuztglmdodevavznxffhicedghwaklcxgesoperfjxcjsczqhexrgeuonsoaetzkxsaowrtgowphaqzjuwcdcwrqriqvbphahhogyjmohdvhwusqtuvvrpnixdfatmactqitzwhjcfqybjqudmswbfgalncgzeisutlvrvetaxrbikynyqkbxsbigfohjzcfupalkhfkeyomimzieljdgmoorevcdxhzpklzrsmvqajboijuwvxchlsbqndvnnvcxidoaplsruthcfdacbcjoumgksezfzdehgsjydbrxxbbcecehgpwdecpygodhrjqzdbzgdtfauieymkgtbewfindrbiyutrtwsvcxojxiubignqzpuxwdkcunmuhgndciyhfosdwmgetwvulrujuglquyndgagiigpouktswbxpxumkngukmhaghnknqswyznovxjowlnnihzvwfpguwhcgwotohrnifddenrayeoinpfkxusrtbtzykumrmbbtfezuwcucseqhkceguvzwrawbvkujmvetbdpzfjcxnufxylesjfqciodfzzonwsgxxxxdgbcvxhtvvzpegggoulvvmliifohdlggthipeskbnlrtrzzivmfryrzgwqnzduhxdsokmylblcuedvmzhuqflpnjfcllysicmyoawzbjexnhucgbwyihoxsvadgyyqckoqlmsovwojfilomjmuscskysogsmzxypnjcfazdokjyxxkgjhymzjmtxunlvkjlyvpfxojnbulytwfeddseogznbmyyxkjxfaccpnpsjcqcavhdbbjriifjvfhurtrwttakjrcsutxuqgathjnpuuapzkjrfapyvslgjnbdiyabgawnljffdhwfrfywhpxfhinrxowiowlwyhmktrpwymdstbqdrpkyzxrxoohemsblbnvuemreawywyrzrgjriijfwxoblgewhrtexgxditcetvfkdoclzlsxkgavoeuyvncstbweuaioxoaaecofpgdzpdxfirdfehyakwefyovxcvzjanopwtrlxoshhymlhmnsktxkiuhmphxpwmhsfyfeoalvyaifmyxzrwjbqtqgcnxybetjeansabnabygpwfqpcarsvhaxwyxyeshjvwbvzryaresnjiclkvgfegebgczlwiwrgzpgsnijtayclbqxywddejelnfykpubvvsdofmxyowdbzvzztecxnikbnileeoccjfqorppinujopioimqfazsdocqvhgxjpcbgwsvggppueaiombvocntsqjnzqkkhplnnditlzigfkqtxtpnvchouqbtiodqdqdquolfpvnvggqdzfsxkzlgsoaaedsjwybqmmxbinwotntsunpgncnhtnjxschqaigoxgpftovmdqgzdygqcvzvkjvenevpoddljuscknqpdxphvxzxsbvemqizwfokjtysnqxagnplkeuhdwfxxcbjumiqrycatavitsibfymsehzidgfcijlcvvumvxnozkyschtjhidlpdvuldwumjyhettzqiprsnkwwfidnlreooycrwyffwmjrikjsbbnrljnsrtgdmmtclvwjtbpsoajyntqqudhhpxbgsjlgwhijeesaghswkduzympuqzbfwfnqobykigtonmovvithmpgmbwhrviytpmlenjoxrilgwabtkubwjjjnhseecdxsrkwgvkdnuotsqxmbtpzfyksgcwkfbkxuwtasfdfasaskzgltvnywxqcaoohsaemxznpeswgmybxyhkrfxrbvhfxxtepuket";
    ch = ashtonString(s, 5665842);

    std::cout << ch << std::endl;
    return 0;
}