package ar.edu.utn.frba.dds.servicioagregador.models.repositories.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Categoria;
import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.ICategoriaRepository;
import org.springframework.stereotype.Repository;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

@Repository
public class CategoriaRepository implements ICategoriaRepository {
  private List<Categoria> categorias;

  private Long obtenerProximaId() { return (long) (categorias.size() + 1); }

  @Override
  public Categoria save(Categoria categoria) {
    if (categoria.getId() == null) {
      categoria.setId(this.obtenerProximaId());
      this.categorias.add(categoria);
    } else {
      this.categorias.remove(this.findById(categoria.getId()));
      this.categorias.add(categoria);
    }
    return categoria;
  }

  @Override
  public List<Categoria> findAll() {
    return this.categorias;
  }

  @Override
  public Categoria findById(Long id) {
    return categorias.stream().filter(c -> c.getId().equals(id)).findFirst().orElse(null);
  }

  @Override
  public void delete(Categoria categoria) {
    this.categorias.remove(categoria);
  }
}
