#include <string>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "YellowPages/YellowPagesDatabase.h"

using testing::Each;

namespace YellowPages
{
    namespace Tests
    {
        MATCHER_P(HasName, companyName, "") {
            for (const auto& name : arg->names)
            {
                if (name.value == companyName)
                {
                    return true;
                }
            }

            *result_listener << "No names with value " << companyName;
            return false;
        };

        class FindCompaniesTest : public testing::Test
        {
        protected:
            FindCompaniesTest() :
                _db(SetUpCompanies(), SetUpRubrics())
            {
            }

            std::unordered_map<BLL::Rubric::IdType, BLL::Rubric> SetUpRubrics() const
            {
                using namespace YellowPages::BLL;
                std::unordered_map<Rubric::IdType, Rubric> _rubrics;
                _rubrics[1] = Rubric{
                    .name = "1",
                    .keywords = {"Вокзал", "ЖД", "Остановка"}
                };
                _rubrics[2] = Rubric{
                    .name = "2",
                    .keywords = {"Парк"}
                };
                _rubrics[3] = Rubric{
                    .name = "3",
                    .keywords = {"Кафе", "Ресторан"}
                };
                _rubrics[4] = Rubric{
                    .name = "4",
                    .keywords = {"Спортзал"}
                };
                _rubrics[5] = Rubric{
                    .name = "5",
                    .keywords = {"Площадь"}
                };
                return _rubrics;
            }

            std::vector<BLL::Company> SetUpCompanies()
            {
                using namespace YellowPages::BLL;
                std::vector<Company> companies;
                // Company 1
                {
                    Company company;
                    company.names = std::vector<Name>{ Name{.value = "OAO Компания1", .type = Name::Main},
                                       Name{.value = "Компания1", .type = Name::Short},
                                       Name{.value = "Синоним", .type = Name::Synonym} };
                    Phone phone1;
                    phone1.type = Phone::PhoneNum;
                    phone1.countryCode = "111";
                    phone1.localCode = "111";
                    phone1.number = "111111111";
                    phone1.extension = "111";

                    Phone phone2;
                    phone2.type = Phone::Fax;
                    phone2.countryCode = "111";
                    phone2.localCode = "111";
                    phone2.number = "111111111";
                    phone2.extension = "111";

                    Phone phone3;

                    company.phones = { phone1, phone2, phone3 };
                    company.urls = { Url{"C1Url1"}, Url{"C1Url2"}, Url{"C1Url3"}, Url{"CommonUrl1"} };
                    company.rubrics = { 1, 2 };


                    companies.emplace_back(company);
                }
                // Company 2
                {
                    Company company;
                    company.names = std::vector<Name>{ Name{.value = "OAO Компания2", .type = Name::Main},
                                       Name{.value = "Компания2", .type = Name::Short},
                                       Name{.value = "Синоним", .type = Name::Synonym} };
                    Phone phone1;
                    phone1.type = Phone::PhoneNum;
                    phone1.countryCode = "222";
                    phone1.localCode = "";
                    phone1.number = "22222222";
                    phone1.extension = "";

                    Phone phone2;
                    phone2.type = Phone::Fax;
                    phone2.countryCode = "222";
                    phone2.localCode = "";
                    phone2.number = "22222222";
                    phone2.extension = "";

                    Phone phone3;
                    phone3.type = Phone::PhoneNum;
                    phone3.number = "3333333";

                    company.phones = { phone1, phone2, phone3 };
                    company.urls = { Url{"C2Url1"}, Url{"C2Url2"}, Url{"C2Url3"}, Url{"CommonUrl1"} };
                    company.rubrics = { 3, 4 };

                    companies.emplace_back(company);
                }
                // Company 3
                {
                    Company company;
                    company.names = std::vector<Name>{ Name{.value = "OAO Компания3", .type = Name::Main} };
                    Phone phone1;
                    phone1.type = Phone::PhoneNum;
                    phone1.countryCode = "333";
                    phone1.localCode = "";
                    phone1.number = "3333333";
                    phone1.extension = "";

                    Phone phone2;
                    phone2.type = Phone::Fax;
                    phone2.countryCode = "333";
                    phone2.localCode = "";
                    phone2.number = "3333333";
                    phone2.extension = "";

                    Phone phone3;
                    phone3.type = Phone::PhoneNum;
                    phone3.number = "3333333";

                    company.phones = { phone1, phone2, phone3 };
                    company.urls = { Url{"C3Url1"}, Url{"C3Url2"}, Url{"C3Url3"}, Url{"CommonUrl1"} };
                    company.rubrics = { 3, 4 };

                    companies.emplace_back(company);
                }
                // Company 4
                {
                    Company company;
                    company.names = std::vector<Name>{ Name{.value = "OAO Компания4", .type = Name::Main},
                                       Name{.value = "Компания4", .type = Name::Short},
                                        Name{.value = "4", .type = Name::Short} };
                    Phone phone1;
                    phone1.type = Phone::PhoneNum;
                    phone1.countryCode = "444";
                    phone1.localCode = "";
                    phone1.number = "4444444";
                    phone1.extension = "";

                    Phone phone2;
                    phone2.type = Phone::Fax;
                    phone2.countryCode = "444";
                    phone2.localCode = "";
                    phone2.number = "4444444";
                    phone2.extension = "";

                    Phone phone3;
                    phone3.type = Phone::PhoneNum;
                    phone3.number = "4444444";

                    company.phones = { phone1, phone2, phone3 };
                    company.urls = { Url{"C4Url1"}, Url{"C4Url2"}, Url{"C4Url3"} };
                    company.rubrics = { 3, 4 };

                    companies.emplace_back(company);
                }
                // Company 5
                {
                    Company company;
                    company.names = std::vector<Name>{ Name{.value = "OAO Компания5", .type = Name::Main},
                                       Name{.value = "Компания5", .type = Name::Short},
                                       Name{.value = "Синоним", .type = Name::Synonym} };
                    Phone phone1;
                    phone1.type = Phone::PhoneNum;
                    phone1.countryCode = "555";
                    phone1.localCode = "";
                    phone1.number = "5555555";
                    phone1.extension = "";

                    Phone phone2;
                    phone2.type = Phone::Fax;
                    phone2.countryCode = "555";
                    phone2.localCode = "";
                    phone2.number = "5555555";
                    phone2.extension = "";

                    Phone phone3;
                    phone3.type = Phone::PhoneNum;
                    phone3.number = "5555555";

                    company.phones = { phone1, phone2, phone3 };
                    company.urls = { Url{"C5Url1"}, Url{"C5Url2"}, Url{"C5Url3"} };
                    company.rubrics = { 3, 4 };

                    companies.emplace_back(company);
                }
                return companies;
            }



        protected:
            YellowPages::BLL::YellowPagesDatabase _db;
        };


        TEST_F(FindCompaniesTest, FindOnlyByNames1)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            std::string companyName = "Компания1";
            companyRestrictions.names = { companyName };
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 1);
            EXPECT_THAT(foundCompanies, Each(HasName(companyName)));
        }

        TEST_F(FindCompaniesTest, FindOnlyByNames2)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.names = { "Компания1", "Компания2" };
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 2);
        }
        
        TEST_F(FindCompaniesTest, FindOnlyByNamesSynonym)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.names = { "Синоним" };
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 3);
        }
        
        TEST_F(FindCompaniesTest, FindOnlyByNames3)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.names = { "Компания1", "Синоним" };
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 3);
        }
        
        TEST_F(FindCompaniesTest, FindOnlyByNames4)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.names = { "OAO Компания3" };
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 1);
        }

        TEST_F(FindCompaniesTest, FindOnlyByUrls1)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.urls = {"C2Url1"};
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 1);
        }
        
        TEST_F(FindCompaniesTest, FindOnlyByUrls2)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.urls = {"CommonUrl1"};
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 3);
        }
        
        TEST_F(FindCompaniesTest, FindOnlyByUrls3)
        {
            using namespace YellowPages::BLL;
            CompanyRestrictions companyRestrictions;
            companyRestrictions.urls = {"CommonUrl1", "C5Url1"};
            auto foundCompanies = _db.FindCompanies(companyRestrictions);
            EXPECT_EQ(foundCompanies.size(), 4);
        }
    }

}