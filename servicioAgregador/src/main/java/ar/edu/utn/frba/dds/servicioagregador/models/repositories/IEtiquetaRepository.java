package ar.edu.utn.frba.dds.servicioagregador.models.repositories;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Etiqueta;
import java.util.List;

public interface IEtiquetaRepository {

  Etiqueta save(Etiqueta etiqueta);
  List<Etiqueta> findAll();
  Etiqueta findById(Long id);
  void delete(Etiqueta etiqueta);
}
