/*********************************************************************************************************************
 * File : test.h                                                                                                     *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef TEST_H
#define TEST_H

namespace test
{
    class Test
    {
    public:
        Test() = default;
        virtual ~Test() = default;

        virtual void on_update(float delta_time) {}
        virtual void on_render() {}
        virtual void on_imgui_render() {}
    };

} // namespace test

#endif // TEST_H
