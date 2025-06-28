package ar.edu.utn.frba.dds.servicioagregador.models.repositories;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Categoria;
import java.util.List;
import java.util.Optional;

public interface ICategoriaRepository {
  Categoria save(Categoria categoria);
  List<Categoria> findAll();
  Categoria findById(Long id);
  void delete(Categoria categoria);
}
