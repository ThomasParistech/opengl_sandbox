/*********************************************************************************************************************
 * File : test_batch_rendering.h                                                                                     *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef TEST_BATCH_RENDERING_H
#define TEST_BATCH_RENDERING_H

#include "test/test.h"

#include "shader.h"
#include "index_buffer.h"
#include "vertex_array.h"

#include <memory>

namespace test
{
    class TestBatchRendering : public Test
    {
    public:
        TestBatchRendering();
        ~TestBatchRendering();

        void on_update(float delta_time) override;
        void on_render() override;
        void on_imgui_render() override;

    private:
        std::unique_ptr<VertexArray> vao_;
        std::unique_ptr<VertexBuffer> vbo_;
        std::unique_ptr<IndexBuffer> ibo_;
        std::unique_ptr<Shader> shader_;
    };

} // namespace test

#endif // TEST_BATCH_RENDERING_H
