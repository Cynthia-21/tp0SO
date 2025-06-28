package ar.edu.utn.frba.dds.servicioagregador.models.repositories.impl;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Etiqueta;
import ar.edu.utn.frba.dds.servicioagregador.models.repositories.IEtiquetaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public class EtiquetaRepository implements IEtiquetaRepository {

  private List<Etiqueta> etiquetas;

  @Override
  public Etiqueta save(Etiqueta etiqueta) {
    return null;
  }

  @Override
  public List<Etiqueta> findAll() {
    return List.of();
  }

  @Override
  public Etiqueta findById(Long id) {
    return etiquetas.stream().filter(e -> e.getId().equals(id)).findFirst().orElse(null);

  }


  @Override
  public void delete(Etiqueta etiqueta) {

  }
}
