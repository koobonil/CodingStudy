#include <boss.hpp>
#include "todaki.hpp"

#include <r.hpp>

ZAY_DECLARE_VIEW_CLASS("todakiView", todakiData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == CT_Tick)
    {
        if(0 < m->Life)
        {
            // 등장
            uint64 CurTime = Platform::Utility::CurrentTimeMsec();
            if(m->LastWordTime + 8000 < CurTime)
            if(m->Words.Count() < 5 && 0 < m->WordPool.LengthOfIndexable())
            {
                const sint32 Index = Platform::Utility::Random() % m->WordPool.LengthOfIndexable();
                chars Key = m->WordPool[Index][0].GetString();
                chars Comment = m->WordPool[Index][1].GetString();
                if(!m->Words.Access(Key))
                {
                    auto& NewWord = m->Words(Key);
                    NewWord.mKey = Key;
                    NewWord.mComment = Comment;
                    NewWord.mPosX = (Platform::Utility::Random() % 1000) / 1000.0f;
                    NewWord.mPosY = -0.1f;
                    switch(Platform::Utility::Random() % 4)
                    {
                    case 0: NewWord.mSpeedX = +0.0009f; break;
                    case 1: NewWord.mSpeedX = +0.0004f; break;
                    case 2: NewWord.mSpeedX = -0.0004f; break;
                    case 3: NewWord.mSpeedX = -0.0009f; break;
                    }
                    switch(Platform::Utility::Random() % 2)
                    {
                    case 0: NewWord.mSpeedY = 0.0001f; break;
                    case 1: NewWord.mSpeedY = 0.0002f; break;
                    }
                    NewWord.mSpeedY += 0.00001f * ((CurTime - m->FirstWordTime) / 1000);
                    NewWord.mDeathCount = 0;
                    m->LastWordTime = CurTime;
                }
            }

            // 애니메이션
            for(sint32 i = m->Words.Count() - 1; 0 <= i; --i)
            {
                chararray GetPath;
                auto CurWord = m->Words.AccessByOrder(i, &GetPath);
                if(CurWord->mDeathCount == 0)
                {
                    CurWord->mPosX += CurWord->mSpeedX;
                    CurWord->mPosY += CurWord->mSpeedY;
                    while(CurWord->mPosX < 0) CurWord->mPosX += 1;
                    while(1 < CurWord->mPosX) CurWord->mPosX -= 1;
                    if(1 < CurWord->mPosY)
                    {
                        if(--m->Life == 0)
                        {
                            Context ScoreContext;
                            ScoreContext.LoadJson(SO_NeedCopy, String::FromFile("json/score.json"));
                            sint32 LastUpdateDay = ScoreContext("LastUpdateDay").GetInt(0);
                            sint32 NewUpdateDay = Platform::Utility::CurrentTimeMsec() / (1000 * 60 * 60 * 24);
                            if(LastUpdateDay == NewUpdateDay)
                            {
                                m->Result_HighScore = ScoreContext("HighScore").GetInt(0);
                                m->Result_TotalScore = ScoreContext("TotalScore").GetInt(0);
                                m->Result_HighScore = Math::Max(m->Result_HighScore, m->Score);
                                m->Result_TotalScore = m->Result_TotalScore + m->Score;
                            }
                            else
                            {
                                m->Result_HighScore = m->Score;
                                m->Result_TotalScore = m->Score;
                            }
                            ScoreContext.At("LastUpdateDay").Set(String::FromInteger(NewUpdateDay));
                            ScoreContext.At("HighScore").Set(String::FromInteger(m->Result_HighScore));
                            ScoreContext.At("TotalScore").Set(String::FromInteger(m->Result_TotalScore));
                            ScoreContext.SaveJson().ToFile("json/score.json");
                        }
                        m->Words.Remove(&GetPath[0]);
                    }
                }
            }
            m->invalidate();
        }
    }
    else if(type == CT_Signal)
    {
        if(0 < m->Life)
        if(!String::Compare("KeyRelease", topic))
        {
            pointers Code(in);
            auto Text = (chars) Code[0];
            if(Text[1] == '\0')
            {
                if('A' <= Text[0] && Text[0] <= 'Z')
                {
                    m->CurText += Text[0];
                    m->try_flush();
                }
                else if(0 < m->CurText.Length())
                {
                    if(Text[0] == 3) // 백스페이스
                    {
                        WString CurTextW = WString::FromChars(m->CurText);
                        CurTextW.Sub(1);
                        m->CurText = String::FromWChars(CurTextW);
                        m->try_flush();
                    }
                    else if(Text[0] == 4 || Text[0] == ' ') // 엔터, 스페이스
                        m->CurText = "";
                }
                m->invalidate();
            }
        }
    }
}

ZAY_VIEW_API OnNotify(chars sender, chars topic, id_share in, id_cloned_share* out)
{
    if(!String::Compare(topic, "BackeyWideCode"))
    {
        wchar_t OldCodeW = 0;
        WString CurTextW = WString::FromChars(m->CurText);
        if(0 < CurTextW.Length())
        {
            OldCodeW = CurTextW[-2];
            CurTextW.Sub(1);
        }

        WString KeyCodeW(in);
        if(KeyCodeW[0] == L'←') // 백스페이스
        {
            m->CurText = String::FromWChars(CurTextW);
            m->try_flush();
        }
        else if(KeyCodeW[0] == L'ⓔ' || KeyCodeW[0] == L'ⓢ') // 엔터, 스페이스
        {
            m->CurText = "";
        }
        else // 문자키
        {
            if(OldCodeW)
            {
                wchars ResultW = WString::MergeKorean(OldCodeW, KeyCodeW[0]);
                CurTextW += ResultW;
            }
            else CurTextW += KeyCodeW[0];
            m->CurText = String::FromWChars(CurTextW);
            m->try_flush();
        }
        m->invalidate();
    }
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
    if(type == GT_Pressed)
    {
        if(m->Life == 0)
        {
            m->init();
            m->invalidate();
        }
    }
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    ZAY_RGB(panel, 200, 255, 255)
        panel.fill();

    // 글자들
    ZAY_FONT(panel, 2.0f, "Arial")
    for(sint32 i = m->Words.Count() - 1; 0 <= i; --i)
    {
        chararray GetPath;
        auto CurWord = m->Words.AccessByOrder(i, &GetPath);
        ZAY_XYRR(panel, panel.w() * CurWord->mPosX, panel.h() * CurWord->mPosY, 15, 15)
        {
            const sint32 DeathCount = (0 < CurWord->mDeathCount)? CurWord->mDeathCount : 11;
            const sint32 AniValue = (sint32) Math::Pow(11 - DeathCount);
            const sint32 OpaValue = (0 < AniValue)? 255 : Math::Clamp(255 * CurWord->mPosY, 0, 255);
            // 타겟글자
            ZAY_FONT(panel, 1.0f + 0.3f * (AniValue % 3))
            {
                ZAY_RGBA(panel, 0, 0, 0, OpaValue)
                    panel.text(0, panel.h() / 2, CurWord->mKey, UIFA_RightMiddle);
                ZAY_RGBA(panel, 0, 0, 0, 255)
                    panel.text(panel.w(), panel.h() / 2, CurWord->mComment, UIFA_LeftMiddle);
            }
            // 폭탄
            ZAY_INNER(panel, 4 - AniValue * 2)
            ZAY_RGBA(panel, 255, 0, 0, 200 - AniValue * 2)
                panel.circle();
            if(0 < CurWord->mDeathCount && --CurWord->mDeathCount == 0)
                m->Words.Remove(&GetPath[0]);
        }
    }

    // 친 글자들
    ZAY_XYWH(panel, 0, panel.h() - 200, panel.w(), 200)
    ZAY_INNER(panel, 10)
    {
        ZAY_RGBA(panel, 0, 0, 0, 32)
            panel.fill();
        ZAY_RGBA(panel, 0, 0, 255, 128)
        ZAY_FONT(panel, 5.0f, "Arial Black")
            panel.text(m->CurText, UIFA_CenterMiddle, UIFE_Left);
    }

    // 점수
    ZAY_FONT(panel, 2.0f, "Arial")
    {
        ZAY_XYWH(panel, 2, 2, panel.w(), panel.h())
        ZAY_INNER(panel, 5)
        ZAY_RGBA(panel, 0, 0, 0, 96)
            panel.text(String::Format("Score: %d", m->Score), UIFA_RightTop, UIFE_Right);
        ZAY_XYWH(panel, 0, 0, panel.w(), panel.h())
        ZAY_INNER(panel, 5)
        ZAY_RGB(panel, 0, 64, 0)
            panel.text(String::Format("Score: %d", m->Score), UIFA_RightTop, UIFE_Right);
    }

    // 생명
    ZAY_FONT(panel, 2.0f, "Arial Black")
    for(sint32 i = 0, iend = m->Life; i < iend; ++i)
    {
        ZAY_XYWH(panel, 60 * i + 2, 2, 60, 50)
        ZAY_INNER(panel, 5)
        ZAY_RGBA(panel, 0, 0, 0, 96)
            panel.text("★", UIFA_CenterMiddle, UIFE_Right);
        ZAY_XYWH(panel, 60 * i, 0, 60, 50)
        ZAY_INNER(panel, 5)
        ZAY_RGB(panel, 255, 192, 0)
            panel.text("★", UIFA_CenterMiddle, UIFE_Right);
    }

    // 게임오버
    if(m->Life == 0)
    {
        ZAY_RGBA(panel, 0, 0, 0, 128)
            panel.fill();
        ZAY_RGBA(panel, 255, 255, 100, 255)
        ZAY_FONT(panel, 4.0f, "Arial")
            panel.text(panel.w() / 2, panel.h() / 2, "GameResult", UIFA_CenterBottom);
        ZAY_RGBA(panel, 255, 255, 255, 255)
        ZAY_FONT(panel, 3.0f, "Arial")
            panel.text(panel.w() / 2, panel.h() / 2, String::Format("HighScore: %d, TotalScore: %d",
                m->Result_HighScore, m->Result_TotalScore), UIFA_CenterTop);
    }

    // 가상키보드
    m->mBackey.Render(panel, m->view());
}

todakiData::todakiData()
{
    WordPool.LoadJson(SO_NeedCopy, String::FromFile("json/words.json"));
    init();
}

todakiData::~todakiData()
{
}

void todakiData::init()
{
    Life = 3;
    Score = 0;
    CurText = "";
    FirstWordTime = Platform::Utility::CurrentTimeMsec();
    LastWordTime = Platform::Utility::CurrentTimeMsec();
    Words.Reset();
    Result_HighScore = 0;
    Result_TotalScore = 0;
}

void todakiData::try_flush()
{
    word* FindWord = Words.Access(CurText);
    for(sint32 i = 0; !FindWord && i < Words.Count(); ++i)
    {
        auto CurWord = Words.AccessByOrder(i);
        if(!CurWord->mComment.Compare(CurText))
            FindWord = CurWord;
    }
    if(FindWord && FindWord->mDeathCount == 0)
    {
        FindWord->mDeathCount = 10;
        Score += FindWord->mKey.Length();
        CurText = "";
    }
}
