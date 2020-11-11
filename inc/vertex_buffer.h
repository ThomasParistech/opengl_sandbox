/*********************************************************************************************************************
 * File : vertex_buffer.h                                                                                            *
 *                                                                                                                   *
 * 2020 Thomas Rouch                                                                                                 *
 *********************************************************************************************************************/

#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class VertexBuffer
{
public:
    VertexBuffer(const void *data, unsigned int bytes_size);

    ~VertexBuffer();

    void bind() const;

    void unbind() const;

private:
    unsigned int rendered_id_;
};

#endif // VERTEX_BUFFER_H
